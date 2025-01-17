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

#ifndef _EXPORTERS_GCE_METADATA_H_
#define _EXPORTERS_GCE_METADATA_H_

#include <cstdint>
#include <string>

#include "absl/container/flat_hash_map.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"

namespace prober {

class GCEMetadata {
 public:
  static absl::StatusOr<absl::flat_hash_map<std::string, std::string>>
  GetGCEMetadata();
};

}  // namespace prober

#endif  // _EXPORTERS_GCE_METADATA_H_
