# Copyright (c) 2020-2021 Advanced Robotics at the University of Washington <robomstr@uw.edu>
#
# This file is part of aruw-edu.
#
# aruw-edu is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# aruw-edu is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with aruw-edu.  If not, see <https://www.gnu.org/licenses/>.

# Configuration reference:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

project = "aruw-edu"
copyright = "2022, Advanced Robotics at the University of Washington"
author = "Advanced Robotics at the University of Washington"

extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.napoleon",
    "sphinx_autodoc_typehints",
    "myst_parser",
    "sphinxcontrib.mermaid",
    "sphinxcontrib.youtube",
    "sphinx_rtd_dark_mode",
]

templates_path = ["_templates"]
exclude_patterns = ["_build", "Thumbs.db", ".DS_Store"]

html_theme = "sphinx_rtd_theme"
html_static_path = ["_static"]

autodoc_default_options = {
    "members": True,
    "show-inheritance": True,
}

# sphinx-autodoc-typehints options
always_document_param_types = True

myst_heading_anchors = 2
myst_enable_extensions = [
    "linkify",
    "colon_fence",
    "amsmath",
    "dollarmath",
    "deflist",
]


# https://stackoverflow.com/questions/23211695/modifying-content-width-of-the-sphinx-theme-read-the-docs
def setup(app):  # type: ignore
    app.add_css_file("theme.css")  # type: ignore
