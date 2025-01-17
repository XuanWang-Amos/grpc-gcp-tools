// Copyright 2023 Google LLC
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _EXPORTERS_EXPORTERS_UTIL_H_
#define _EXPORTERS_EXPORTERS_UTIL_H_

#include <cstdint>
#include <string>

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "loader/exporter/data_types.h"

namespace prober {

class ExportersUtil {
 public:
  static absl::StatusOr<std::string> GetLogString(std::string& log_name,
                                                  std::string uuid,
                                                  const void* const data);
  static uint64_t GetLogConnId(std::string& log_name, const void* const data);
  static absl::StatusOr<std::string> GetMetricString(std::string name,
                                                     std::string uuid,
                                                     const MetricDesc& desc,
                                                     const void* const key,
                                                     const void* const value);
  static absl::Time GetLogTime(std::string& log_name, const void* const data);
  static absl::Time GetTimeFromBPFns(uint64_t timestamp);
  static int64_t GetMetric(const void* const data, MetricType type);
};

class MetricTimeChecker {
 public:
  MetricTimeChecker() = default;
  // The Checker returns the last metric timestamp or error
  absl::StatusOr<uint64_t> CheckMetricTime(std::string& metric_name,
                                           std::string uuid,
                                           uint64_t timestamp);
  absl::StatusOr<uint64_t> GetMetricStartTime(std::string& metric_name,
                                              std::string uuid);
  absl::StatusOr<uint64_t> GetMetricTime(std::string& metric_name,
                                         std::string uuid);
  absl::flat_hash_set<std::string> GetUUID();
  void DeleteValue(std::string uuid);

 private:
  absl::flat_hash_map<std::string, absl::flat_hash_map<std::string, uint64_t> >
      last_read_;
  absl::flat_hash_map<std::string, absl::flat_hash_map<std::string, uint64_t> >
      start_read_;
  absl::flat_hash_set<std::string> uuids_;
};

class MetricDataMemory {
 public:
  MetricDataMemory() = default;
  // The Checker returns the last metric timestamp or error
  uint64_t StoreAndGetValue(std::string& metric_name, std::string uuid,
                            uint64_t data);
  absl::flat_hash_set<std::string> GetUUID();
  void DeleteValue(std::string uuid);

 private:
  absl::flat_hash_map<std::string, absl::flat_hash_map<std::string, uint64_t> >
      data_memory_;
  absl::flat_hash_set<std::string> uuids_;
};

}  // namespace prober

#endif  // _EXPORTERS_EXPORTERS_UTIL_H_
