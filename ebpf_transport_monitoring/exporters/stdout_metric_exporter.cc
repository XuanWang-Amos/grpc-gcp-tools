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

#include "exporters/stdout_metric_exporter.h"

#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

#include "absl/status/status.h"
#include "events.h"
#include "exporters/exporters_util.h"

namespace prober {

absl::Status StdoutMetricExporter::RegisterMetric(std::string name,
                                                  const MetricDesc& desc) {
  if (metrics_.find(name) != metrics_.end()) {
    return absl::AlreadyExistsError("metric_name already registered");
  }
  metrics_[name] = desc;
  return absl::OkStatus();
}

absl::Status StdoutMetricExporter::HandleData(std::string metric_name,
                                              void* key, void* value) {
  auto it = metrics_.find(metric_name);
  if (it == metrics_.end()) {
    return absl::NotFoundError("metric_name not found");
  }

  metric_format_t* metric = (metric_format_t*)value;

  auto uuid = correlator_->GetUUID(*(uint64_t*)key);

  if (!uuid.ok()) {
    return absl::OkStatus();
  }

  if (!last_read_.CheckMetricTime(metric_name, *uuid, metric->timestamp).ok()) {
    return absl::OkStatus();
  }

  auto metric_str = ExportersUtil::GetMetricString(
      metric_name, *uuid, it->second, key, &(metric->data));
  if (!metric_str.ok()) {
    return metric_str.status();
  }
  std::cout << *metric_str << std::endl;
  return absl::OkStatus();
}

void StdoutMetricExporter::Cleanup() {
  auto uuids = last_read_.GetUUID();
  for (auto uuid : uuids) {
    if (!correlator_->CheckUUID(uuid)) {
      last_read_.DeleteValue(uuid);
    }
  }
}

}  // namespace prober
