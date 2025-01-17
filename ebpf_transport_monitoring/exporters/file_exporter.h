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

#ifndef _EXPORTERS_FILE_EXPORTER_H_
#define _EXPORTERS_FILE_EXPORTER_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "exporters/exporters_util.h"
#include "loader/exporter/log_exporter.h"
#include "loader/exporter/metric_exporter.h"
#include "spdlog/spdlog.h"

namespace prober {

class FileLogger : public LogExporterInterface {
 public:
  FileLogger();
  FileLogger(uint8_t max_files, uint32_t file_size, std::string dir_name);
  ~FileLogger() { spdlog::shutdown(); }
  absl::Status Init() override;

  absl::Status RegisterLog(std::string name, LogDesc& log_desc) override;
  absl::Status HandleData(std::string log_name, const void* const data,
                          const uint32_t size) override;

 private:
  absl::flat_hash_map<std::string, bool> logs_;
  uint8_t max_files_;
  uint32_t file_size_;
  std::string directory_;
  std::shared_ptr<spdlog::logger> logger_;
};

class FileMetricExporter : public MetricExporterInterface {
 public:
  FileMetricExporter();
  FileMetricExporter(uint8_t max_files, uint32_t file_size,
                     std::string dir_name);
  ~FileMetricExporter() { spdlog::shutdown(); }
  absl::Status Init() override;
  absl::Status RegisterMetric(std::string name,
                              const MetricDesc& desc) override;
  absl::Status HandleData(std::string metric_name, void* key,
                          void* value) override;
  void Cleanup();

 private:
  absl::flat_hash_map<std::string, MetricDesc> metrics_;
  MetricTimeChecker last_read_;
  uint8_t max_files_;
  uint32_t file_size_;
  std::string directory_;
  std::shared_ptr<spdlog::logger> logger_;
};

}  // namespace prober

#endif  // _EXPORTERS_FILE_EXPORTER_H_
