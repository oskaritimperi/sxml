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

#ifndef SXML_H
#define SXML_H

#include <string>
#include <sstream>
#include <map>
#include <vector>

namespace sxml {

class element;

typedef std::vector<element> element_list;
typedef std::map<std::string, std::string> attribute_map;

class element
{
    public:
    
    //! Constructs an empty element (i.e., no name,
    //! children, attributes).
    element();
    
    //! Copy constructor.
    element(const element &elem);
    
    //! Construcst an element with the specified name and text.
    element(const std::string &name);
  
    //! Creates a textual representation of the element. If nice
    //! is true, the returned string is formatted with indentations
    //! and newlines.
    std::string to_string(bool nice = false, int indent = 0) const;
    
    //! Adds a child element to this element. 
    element &add_child(const element &child);
    
    //! Set the text for this element. An element can either have text
    //! or children. If an element has both, children take precedence
    //! when calling to_string().
    template<class T>
    element &set_text(const T &text);

    //! Set an attribute for this element with the specified name and
    //! value.
    template<class T>
    element &set_attr(const std::string &name, const T &value);

    private:
    
    element_list m_children;
    attribute_map m_attributes;
    
    std::string m_name;
    std::string m_text;
};

template<class T> element &element::set_text(const T &text)
{
    std::string s;
    std::stringstream ss(s);
    ss << text;

    return set_text<std::string>(ss.str());
}

template<> element &element::set_text<>(const std::string &text);

template<class T>
element &element::set_attr(const std::string &name, const T &value)
{
    std::string s;
    std::stringstream ss(s);
    ss << value;

    return set_attr<std::string>(name, ss.str());
}

template<>
element &element::set_attr(const std::string &name, const std::string &value);

} // namespace sxml

#endif // SXML_H

