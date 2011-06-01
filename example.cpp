/*
 * This file is part of sxml - stupid XML generator
 *
 * Copyright (c) 2011 Oskari Timperi
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute
 * it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must
 * not claim that you wrote the original software. If you use this
 * software in a product, an acknowledgment in the product documentation
 * would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must
 * not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 */

#include <iostream>

#include "sxml.h"

using sxml::element;
using sxml::comment;
int main()
{
    element *root = new element("root");

    root->add_element("node")->set_attr("id", 0);

    root->add_comment("yay, a comment");
    root->add_element("node")->set_attr("id", 1);

    element *node = root->add_element("node")->set_attr("id", 3.1);
    node = node->add_element("subnode");
    node->add_element("subsubnode")->add_comment(
        "i am in pretty deep waters here, man ..");

    std::cout << root->to_string(true) << std::endl;

    return 0;
}

