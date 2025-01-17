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

#include "sources/source_manager/tcp_source.h"

#include <bpf/libbpf.h>
#include <linux/bpf.h>

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "loader/source/data_source.h"
#include "loader/source/source_helper.h"

namespace prober {

// File name is hardcoded with a relative location for now.
// Will do something better later.
TcpSource::TcpSource() {
  MetricUnit_t bytes;
  bytes.type = MetricUnitType::kData;
  bytes.data = MetricDataType::kbytes;

  MetricUnit_t usec;
  usec.type = MetricUnitType::kTime;
  usec.time = MetricTimeType::kusec;
  init_ = false;
  log_sources_ = {
      new DataCtx("tcp_events", LogDesc{}, absl::Seconds(2), false, false)};
  metric_sources_ = {
      new DataCtx("tcp_retransmits",
                  MetricDesc{MetricType::kUint64,
                             MetricType::kUint32,
                             MetricKind::kCumulative,
                             {MetricUnitType::kNone}},
                  absl::Seconds(60), false, false),
      new DataCtx("tcp_rtt",
                  MetricDesc{MetricType::kUint64, MetricType::kUint32,
                             MetricKind::kDistribution, usec},
                  absl::Seconds(10), false, false),
      new DataCtx("tcp_snd_bytes",
                  MetricDesc{MetricType::kUint64, MetricType::kUint32,
                             MetricKind::kCumulative, bytes},
                  absl::Seconds(60), false, false),
      new DataCtx("tcp_rcv_bytes",
                  MetricDesc{MetricType::kUint64, MetricType::kUint32,
                             MetricKind::kCumulative, bytes},
                  absl::Seconds(60), false, false),
      new DataCtx("tcp_snd_cwnd",
                  MetricDesc{MetricType::kUint64,
                             MetricType::kUint32,
                             MetricKind::kGauge,
                             {MetricUnitType::kNone}},
                  absl::Seconds(60), false, false),
      new DataCtx("tcp_rcv_cwnd",
                  MetricDesc{MetricType::kUint64, MetricType::kUint32,
                             MetricKind::kGauge, bytes},
                  absl::Seconds(60), false, false),
      new DataCtx("tcp_pid_filter",
                  MetricDesc{MetricType::kUint64, MetricType::kUnit8,
                             MetricKind::kNone, bytes},
                  absl::Seconds(60), true, false),
      new DataCtx("tcp_connection",
                  MetricDesc{MetricType::kUint64, MetricType::kInternal,
                             MetricKind::kNone, bytes},
                  absl::Seconds(60), true, false),
  };

  pid_filter_map_ = "tcp_pid_filter";

  if (SourceHelper::TestProgType(BPF_PROG_TYPE_RAW_TRACEPOINT)) {
    std::cout << "Loading raw_tracepoint" << std::endl;
    probes_ = {
        new RawTPProbe("sock_state", "sock", "inet_sock_set_state"),
        // This is a better probe but it is causing a lot of incorrect entries
        // for now. So for now we will use a kprobe
        new RawTPProbe("tcp_congestion", "tcp", "tcp_probe"),
        new RawTPProbe("tcp_retransmit", "tcp", "tcp_retransmit_skb"),
        new RawTPProbe("tcp_send_reset", "tcp", "tcp_send_reset"),
        new RawTPProbe("tcp_receive_reset", "tcp", "tcp_receive_reset"),
        new KProbe("probe_tcp_sendmsg", "tcp_sendmsg", false),
    };

    file_name_ = "./tcp_bpf.o";
    file_name_core_ = "./tcp_bpf_core.o";
  } else {
    probes_ = {
        new KProbe("probe_tcp_sendmsg", "tcp_sendmsg", false),
        new KProbe("probe_tcp_set_state", "tcp_set_state", false),
    };

    file_name_ = "./tcp_bpf_kprobe.o";
    file_name_core_ = "./tcp_bpf_kprobe_core.o";
  }
}

TcpSource::~TcpSource() { DataSource::Cleanup(); }
}  // namespace prober
