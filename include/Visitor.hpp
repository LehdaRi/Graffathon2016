#ifndef VISITOR_HPP
#define VISITOR_HPP


template <typename T_Visitor, typename T_Component>
class Visitor {
public:
    void operator()(T_Component& component);
};


template <typename T_Visitor, typename T_Component>
void Visitor<T_Visitor, T_Component>::operator()(T_Component& component) {
    (*static_cast<T_Visitor*>(this))(component);
}


#endif  //  VISITOR_HPP
