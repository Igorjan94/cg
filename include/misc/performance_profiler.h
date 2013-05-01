#pragma once
#include <chrono>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <string>

namespace util
{
    namespace perftest
    {
        class performance_profiler
        {
        public:
            performance_profiler(std::string const& name, bool writeStats = true, std::ostream& logStream = std::cout)
                : _name(name),
                  _writeStats(writeStats),
                  _logStream(logStream),
                  _start(std::chrono::high_resolution_clock::now())
            {
                performance_profiler::history.push(name);
            }

            ~performance_profiler()
            {
                using namespace std::chrono;
                auto end = high_resolution_clock::now();
                auto time = duration_cast<duration<double>>(end - _start);

                performance_profiler::history.pop();
                auto statsIterator = performance_profiler::statistics.find(_name);

                if (statsIterator == performance_profiler::statistics.end())
                {
                    performance_profiler::statistics[_name] = {1, time.count()};
                }
                else
                {
                    ++(statsIterator->second.first);
                    statsIterator->second.second += time.count();
                }

                if (!performance_profiler::history.empty())
                {
                    auto parent = performance_profiler::history.top();
                    auto innerIterator = performance_profiler::innerStatistics[parent].find(_name);

                    if (innerIterator == performance_profiler::innerStatistics[parent].end())
                    {
                        performance_profiler::innerStatistics[parent].insert( {_name, {1, time.count()}});
                    }
                    else
                    {
                        ++(innerIterator->second.first);
                        innerIterator->second.second += time.count();
                    }
                }

                if (_writeStats)
                {
                    auto sumStat = performance_profiler::statistics[_name];

                    _logStream << "-------\nStatistics for test: \""
                               << _name
                               << "\".\nSingle execution time: "
                               << time.count()
                               << " seconds.\nSummary statistics:\n Run count: "
                               << sumStat.first
                               << "\n Summary time: "
                               << sumStat.second
                               << " seconds.\n Average time: "
                               << (time.count() / sumStat.first)
                               << " seconds.\n"
                               << "Inner tests:\n";

                    for (auto it = performance_profiler::innerStatistics[_name].begin();
                            it != performance_profiler::innerStatistics[_name].end(); ++it)
                    {
                        _logStream << " Name: \""
                                   << it->first
                                   << "\".\n  Executed "
                                   << it->second.first
                                   << " times.\n  Summary time: "
                                   << it->second.second
                                   << " seconds.\n  Average time: "
                                   << (it->second.second / it->second.first)
                                   << " seconds.\n";
                    }

                    _logStream << "-------\n";
                }
            }

        private:
            std::string _name;
            bool _writeStats;
            std::ostream& _logStream;
            std::chrono::high_resolution_clock::time_point _start;

            static std::unordered_map<std::string, std::pair<int, double>> statistics;
            static std::unordered_map<std::string, std::unordered_map<std::string, std::pair<int, double>>> innerStatistics;
            static std::stack<std::string> history;
        };

        std::unordered_map<std::string, std::pair<int, double>> performance_profiler::statistics;
        std::unordered_map<std::string, std::unordered_map<std::string, std::pair<int, double>>> performance_profiler::innerStatistics;
        std::stack<std::string> performance_profiler::history;
    }
}
