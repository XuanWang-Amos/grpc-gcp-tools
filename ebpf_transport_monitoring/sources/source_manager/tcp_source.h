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

#ifndef _SOURCES_TCP_SOURCE_H_
#define _SOURCES_TCP_SOURCE_H_

#include <string>

#include "loader/source/data_source.h"

namespace prober {

class TcpSource : public DataSource {
 public:
  TcpSource();
  ~TcpSource() override;
  std::string ToString() const override { return "TcpSource"; };
};

}  // namespace prober

#endif  // _SOURCES_TCP_SOURCE_H_
