// The GPLv3 License (GPLv3)

// Copyright (c) 2023 Marvin Frohwitter

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <algorithm>
#include <bits/types/FILE.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>

std::string splithexstring(std::string, std::list<std::string> *, std::string);
std::string getcolor(std::string, std::list<std::string> *);
void write_to_yaml_file(std::list<std::string> *, std::string);

int main(int argc, char *argv[]) {
  if (argc < 2 || argc > 2) {
    std::cout << "Incorrrect argument number was given!" << std::endl;
    exit(0);
  }
  const std::string filename = argv[1];
  FILE *subprocess_in;
  try {
    subprocess_in = popen("xrdb -query", "r");
  } catch (const std::exception &) {
    std::cout << "Calling xrdb -query went wrong" << std::endl;
  }

  std::list<std::string> colorlist;
  char buff[128];
  while (fgets(buff, sizeof(buff), subprocess_in)) {
    std::string inputline = fgets(buff, sizeof(buff), subprocess_in);
    colorlist.push_back(inputline);
  }

  write_to_yaml_file(&colorlist, filename);
  return 0;
}

std::string splithexstring(std::string color_name,
                           std::list<std::string> *xr_colors,
                           std::string delimeter) {

  while (xr_colors->size() >= 0) {
    std::string hexcode;
    std::string front = xr_colors->front();
    front.erase(0, 1);
    front.erase(front.find_first_of(":"), front.length());

    if (front != color_name) {
      xr_colors->pop_front();
    } else {
      std::string string_with_name_and_hexcode = xr_colors->front();
      int start_hex = string_with_name_and_hexcode.find(delimeter) + 1;
      string_with_name_and_hexcode.erase(0, start_hex + 1);
      hexcode = string_with_name_and_hexcode;
      // std::cout << hexcode << std::endl;
      return hexcode;
    }
  }
  return "";
}
std::string getcolor(std::string color_name,
                     std::list<std::string> *xr_colors) {
  for (auto var : *xr_colors) {
    std::cout << var << std::endl;
  }

  for (std::string color : *xr_colors) {
    color.erase(color.find("\n"), color.length());
    color.erase(0, 1);
    color.erase(color.find(":"), color.length());

    std::cout << color << std::endl;
    if (color == color_name) {
      std::cout << color << std::endl;
      return splithexstring(color_name, xr_colors, "\t");
    }
  }
  return "";
}

void write_to_yaml_file(std::list<std::string> *xr_colors,
                        std::string filename) {
  YAML::Node config = YAML::LoadFile(filename);

  config["colors"]["primary"]["background"] = getcolor("background", xr_colors);
  config["colors"]["primary"]["foreground"] = getcolor("foreground", xr_colors);

  config["colors"]["normal"]["black"] = getcolor("color0", xr_colors);
  config["colors"]["normal"]["red"] = getcolor("color1", xr_colors);
  config["colors"]["normal"]["green"] = getcolor("color2", xr_colors);
  config["colors"]["normal"]["yellow"] = getcolor("color3", xr_colors);
  config["colors"]["normal"]["blue"] = getcolor("color4", xr_colors);
  config["colors"]["normal"]["magenta"] = getcolor("color5", xr_colors);
  config["colors"]["normal"]["cyan"] = getcolor("color6", xr_colors);
  config["colors"]["normal"]["white"] = getcolor("color7", xr_colors);

  config["colors"]["bright"]["black"] = getcolor("color8", xr_colors);
  config["colors"]["bright"]["red"] = getcolor("color9", xr_colors);
  config["colors"]["bright"]["green"] = getcolor("color10", xr_colors);
  config["colors"]["bright"]["yellow"] = getcolor("color11", xr_colors);
  config["colors"]["bright"]["blue"] = getcolor("color12", xr_colors);
  config["colors"]["bright"]["magenta"] = getcolor("color13", xr_colors);
  config["colors"]["bright"]["cyan"] = getcolor("color14", xr_colors);
  config["colors"]["bright"]["white"] = getcolor("color15", xr_colors);

  std::ofstream fout(filename);
  fout << config;
  fout.close();
}
