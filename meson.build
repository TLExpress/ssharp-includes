# Copyright (C) 2025 TLExpress.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

project('ssharp-includes', meson_version: '>=1.7.0')

ssharp_includes_dep = declare_dependency(
    include_directories: include_directories('.'),
    version: meson.project_version(),
)

meson.override_dependency('ssharp-includes', ssharp_includes_dep)
