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

#include <bits/types/FILE.h>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>

std::string getcolor(std::string, std::list<std::string> *);
std::string splithexstring(std::string *, std::string *, std::string);
void write_to_yaml_file(std::string, std::list<std::string> *);

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

  write_to_yaml_file(filename, &colorlist);
  pclose(subprocess_in);
  return 0;
}

std::string splithexstring(std::string *color_name, std::string *color_hex,
                           std::string delimeter) {

  color_hex->erase(0, color_hex->find(delimeter) + 1);
  return color_hex->erase(color_hex->find('\n'), color_hex->length());
}

std::string getcolor(std::string color_name,
                     std::list<std::string> *xr_colors) {
  for (auto colors : *xr_colors) {
    if (colors.find(color_name) != std::string::npos) {
      return splithexstring(&color_name, &colors, "\t");
    }
  }
  return "";
}

void write_to_yaml_file(std::string filename,
                        std::list<std::string> *xr_colors) {
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
