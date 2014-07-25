//------------------------------------------------------------------------------------------------------------------
#define Q_DECLARE_RTTI \
public: \
    static const ClassLabel Class; \
    virtual const ClassLabel& GetClass () const { return Class; }; \
protected:
//------------------------------------------------------------------------------------------------------------------
#define Q_IMPLEMENT_RTTI(nsname,baseclassname,classname) \
    const ClassLabel classname::Class(#nsname"."#classname,&baseclassname::Class); \
//------------------------------------------------------------------------------------------------------------------
#define Q_DECLARE_TEMPLATE_RTTI \
public: \
    Q_ITEM static const ClassLabel Class; \
    virtual const ClassLabel& GetClass () const { return Class; }; \
protected:
//------------------------------------------------------------------------------------------------------------------
#define Q_IMPLEMENT_TEMPLATE_RTTI(nsname,baseclassname,classname) \
    template <> \
    const ClassLabel classname::Class(#nsname"."#classname,&baseclassname::Class); \
//------------------------------------------------------------------------------------------------------------------



