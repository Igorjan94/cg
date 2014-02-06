// From https://github.com/BorisMinaev/cg
#pragma once

#include "cg/primitives/point.h"
#include <boost/numeric/interval.hpp>
#include <gmpxx.h>

#include <boost/optional.hpp>

namespace cg
{

   struct compare_dist_d
   {
      boost::optional<bool> operator() (point_2 const & a, point_2 const & b, point_2 const & c, point_2 const & d) const
      {
         return boost::none;
         double dx1 = a.x - b.x;
         double dx2 = c.x - d.x;
         double dy1 = a.y - b.y;
         double dy2 = c.y - d.y;
         double sq1 = dx1*dx1+dy1*dy1;
         double sq2 = dx2*dx2+dy2*dy2;
         double sum = sq1+sq2;
         double eps = sum * 8 * std::numeric_limits<double>::epsilon();
         double diff = sq1 - sq2;

         if (diff > eps)
         {
            return false;
         }

         if (diff < -eps)
         {
            return true;
         }

         return boost::none;
      }
   };

   struct compare_dist_i
   {
      boost::optional<bool> operator() (point_2 const & a, point_2 const & b, point_2 const & c, point_2 const & d) const
      {
         typedef boost::numeric::interval_lib::unprotect<boost::numeric::interval<double> >::type interval;
         boost::numeric::interval<double>::traits_type::rounding _;
         interval dx1 = interval(a.x) - b.x;
         interval dx2 = interval(c.x) - d.x;
         interval dy1 = interval(a.y) - b.y;
         interval dy2 = interval(c.y) - d.y;
         interval sq1 = dx1*dx1+dy1*dy1;
         interval sq2 = dx2*dx2+dy2*dy2;
         interval diff = sq1 - sq2;

         if (diff.lower() > 0)
         {
            return false;
         }

         if (diff.upper() < 0)
         {
            return true;
         }

         return boost::none;
      }
   };

   struct compare_dist_r
   {
      boost::optional<bool> operator() (point_2 const & a, point_2 const & b, point_2 const & c, point_2 const & d) const
      {
         mpq_class dx1 = mpq_class(a.x) - b.x;
         mpq_class dx2 = mpq_class(c.x) - d.x;
         mpq_class dy1 = mpq_class(a.y) - b.y;
         mpq_class dy2 = mpq_class(c.y) - d.y;
         mpq_class sq1 = dx1*dx1+dy1*dy1;
         mpq_class sq2 = dx2*dx2+dy2*dy2;
         mpq_class diff = sq1 - sq2;
         return diff < 0;
      }
   };

   // return true, if |ab| < |cd|
   inline bool compare_dist(point_2 const & a, point_2 const & b, point_2 const & c, point_2 const & d)
   {
      if (boost::optional<bool> v = compare_dist_d()(a, b, c, d))
      {
         return *v;
      }

      if (boost::optional<bool> v = compare_dist_i()(a, b, c, d))
      {
         return *v;
      }

      return *compare_dist_r()(a, b, c, d);
   }
}
