// quick-lint-js finds bugs in JavaScript programs.
// Copyright (C) 2020  Matthew Glazar
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <iosfwd>
#include <ostream>
#include <quick-lint-js/char8.h>
#include <string>
#include <type_traits>

namespace quick_lint_js {
template <class Char>
void write_json_escaped_string(std::ostream &output,
                               std::basic_string_view<Char> string) {
  auto write_string = [&](std::basic_string_view<Char> s) {
    if constexpr (std::is_same_v<Char, char>) {
      output << s;
    } else {
      output << out_string8(s);
    }
  };

  for (;;) {
    auto special_character_index =
        string.find_first_of(reinterpret_cast<const Char *>(u8"\\\""));
    if (special_character_index == string.npos) {
      break;
    }
    write_string(string.substr(0, special_character_index));
    output << '\\' << static_cast<char>(string[special_character_index]);
    string = string.substr(special_character_index + 1);
  }
  write_string(string);
}

template void write_json_escaped_string<char>(std::ostream &,
                                              std::basic_string_view<char>);
template void write_json_escaped_string<char8>(std::ostream &,
                                               std::basic_string_view<char8>);
}
