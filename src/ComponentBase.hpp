#ifndef COMPONENTBASE_HPP
#define COMPONENTBASE_HPP


class ComponentBase {
public:
    friend class Scene;

    ComponentBase(void);

    void invalidate(void);

    //type-id information
    template <typename T_Component>
    static unsigned getTypeId(void);

private:
    bool valid_;
    bool active_;

    static unsigned typeIdCounter__;
};


template <typename T_Component>
unsigned ComponentBase::getTypeId(void) {
    static unsigned typeId = typeIdCounter__++;
    return typeId;
}


#endif  //  COMPONENTBASE_HPP
