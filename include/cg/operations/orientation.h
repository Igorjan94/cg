#pragma once

#include "cg/primitives/point.h"
#include "cg/primitives/contour.h"
#include <boost/numeric/interval.hpp>
#include <gmpxx.h>

#include <boost/optional.hpp>

namespace cg
{
   enum orientation_t
   {
      CG_RIGHT = -1,
      CG_COLLINEAR = 0,
      CG_LEFT = 1
   };

   inline bool opposite(orientation_t a, orientation_t b)
   {
      if (a == CG_COLLINEAR || b == CG_COLLINEAR)
      {
         return false;
      }

      return a == -b;
   }

   template <class Scalar>
   struct orientation_d
   {
      boost::optional<orientation_t> operator() (point_2t<Scalar> const & a, point_2t<Scalar> const & b, point_2t<Scalar> const & c) const
      {
         Scalar l = (b.x - a.x) * (c.y - a.y);
         Scalar r = (b.y - a.y) * (c.x - a.x);
         Scalar res = l - r;
         Scalar eps = (fabs(l) + fabs(r)) * 8 * std::numeric_limits<Scalar>::epsilon();

         if (res > eps)
         {
            return CG_LEFT;
         }

         if (res < -eps)
         {
            return CG_RIGHT;
         }

         return boost::none;
      }

      boost::optional<orientation_t> operator() (point_2t<Scalar> const & a, point_2t<Scalar> const & b, point_2t<Scalar> const & c, point_2t<Scalar> const & d) const
      {
         Scalar l = (b.x - a.x) * (d.y - c.y);
         Scalar r = (b.y - a.y) * (d.x - c.x);
         Scalar res = l - r;
         Scalar eps = (fabs(l) + fabs(r)) * 8 * std::numeric_limits<Scalar>::epsilon();

         if (res > eps)
         {
            return CG_LEFT;
         }

         if (res < -eps)
         {
            return CG_RIGHT;
         }

         return boost::none;
      }

   };

   template <class Scalar>
   struct orientation_i
   {
      boost::optional<orientation_t> operator() (point_2t<Scalar> const & a, point_2t<Scalar> const & b, point_2t<Scalar> const & c) const
      {
         typedef typename boost::numeric::interval_lib::unprotect<typename boost::numeric::interval<Scalar> >::type interval;

         typename boost::numeric::interval<Scalar>::traits_type::rounding _;
         interval res =   (interval(b.x) - a.x) * (interval(c.y) - a.y)
                          - (interval(b.y) - a.y) * (interval(c.x) - a.x);

         if (res.lower() > 0)
         {
            return CG_LEFT;
         }

         if (res.upper() < 0)
         {
            return CG_RIGHT;
         }

         if (res.upper() == res.lower())
         {
            return CG_COLLINEAR;
         }

         return boost::none;
      }

      boost::optional<orientation_t> operator() (point_2t<Scalar> const & a, point_2t<Scalar> const & b, point_2t<Scalar> const & c, point_2t<Scalar> const & d) const
      {
         typedef typename boost::numeric::interval_lib::unprotect<typename boost::numeric::interval<Scalar> >::type interval;

         typename boost::numeric::interval<Scalar>::traits_type::rounding _;
         interval res =   (interval(b.x) - a.x) * (interval(d.y) - c.y)
                          - (interval(b.y) - a.y) * (interval(d.x) - c.x);

         if (res.lower() > 0)
         {
            return CG_LEFT;
         }

         if (res.upper() < 0)
         {
            return CG_RIGHT;
         }

         if (res.upper() == res.lower())
         {
            return CG_COLLINEAR;
         }

         return boost::none;
      }

   };

   template <class Scalar>
   struct orientation_r
   {
      boost::optional<orientation_t> operator() (point_2t<Scalar> const & a, point_2t<Scalar> const & b, point_2t<Scalar> const & c) const
      {
         mpq_class res =   (mpq_class(b.x) - a.x) * (mpq_class(c.y) - a.y)
                           - (mpq_class(b.y) - a.y) * (mpq_class(c.x) - a.x);

         int cres = cmp(res, 0);

         if (cres > 0)
         {
            return CG_LEFT;
         }

         if (cres < 0)
         {
            return CG_RIGHT;
         }

         return CG_COLLINEAR;
      }

      boost::optional<orientation_t> operator() (point_2t<Scalar> const & a, point_2t<Scalar> const & b, point_2t<Scalar> const & c, point_2t<Scalar> const & d) const
      {
         mpq_class res =   (mpq_class(b.x) - a.x) * (mpq_class(d.y) - c.y)
                           - (mpq_class(b.y) - a.y) * (mpq_class(d.x) - c.x);

         int cres = cmp(res, 0);

         if (cres > 0)
         {
            return CG_LEFT;
         }

         if (cres < 0)
         {
            return CG_RIGHT;
         }

         return CG_COLLINEAR;
      }

   };

   template <class Scalar>
   inline orientation_t orientation(point_2t<Scalar> const & a, point_2t<Scalar> const & b, point_2t<Scalar> const & c)
   {
      if (boost::optional<orientation_t> v = orientation_d<Scalar>()(a, b, c))
      {
         return *v;
      }

      if (boost::optional<orientation_t> v = orientation_i<Scalar>()(a, b, c))
      {
         return *v;
      }

      return *orientation_r<Scalar>()(a, b, c);
   }

   template <class Scalar>
   inline orientation_t orientation(point_2t<Scalar> const & a, point_2t<Scalar> const & b, point_2t<Scalar> const & c, point_2t<Scalar> const & d)
   {
      if (boost::optional<orientation_t> v = orientation_d<Scalar>()(a, b, c, d))
      {
         return *v;
      }

      if (boost::optional<orientation_t> v = orientation_i<Scalar>()(a, b, c, d))
      {
         return *v;
      }

      return *orientation_r<Scalar>()(a, b, c);
   }
   template <class Scalar>
   inline bool counterclockwise(contour_2t<Scalar> const & c)
   {
      if (c.size() < 3)
      {
         return true;
      }

      auto it_min_point = std::min_element(c.begin(), c.end());

      auto min_point = *it_min_point;

      auto it_prev = --c.circulator(it_min_point);
      auto it_next = ++c.circulator(it_min_point);

      point_2t<Scalar> prev = *it_prev;
      point_2t<Scalar> next = *it_next;

      return orientation(prev, min_point, next) == CG_LEFT;
   }

   template <class Scalar>
   inline bool collinear_are_ordered_along_line(point_2t<Scalar> const & a, point_2t<Scalar> const & b, point_2t<Scalar> const & c)
   {
      return (a <= b && b <= c) || (c <= b && b <= a);
   }
}
