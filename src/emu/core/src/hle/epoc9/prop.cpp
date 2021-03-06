#include <epoc9/prop.h>

using namespace eka2l1;

BRIDGE_FUNC(TInt, RPropertyAttach, eka2l1::ptr<RProperty> aProp, TUid aCagetory, TUint aKey, TOwnerType aType) {
    memory_system *mem = sys->get_memory_system();
    kernel_system *kern = sys->get_kernel_system();

    RProperty *prop_lle = aProp.get(mem);

    property_ptr prop_hle = kern->get_prop(aCagetory, aKey);

    if (!prop_hle) {
        // Still success
        return KErrNone;
    }

    prop_lle->iHandle = prop_hle->unique_id();
    prop_hle->set_owner_type(aType == EOwnerProcess ? kernel::owner_type::process : kernel::owner_type::thread);

    return KErrNone;
}

BRIDGE_FUNC(TInt, RPropertyCancel, eka2l1::ptr<RProperty> aProp) {
    memory_system *mem = sys->get_memory_system();
    kernel_system *kern = sys->get_kernel_system();

    RProperty *prop_lle = aProp.get(mem);

    property_ptr prop_hle = kern->get_prop(prop_lle->iHandle);

    if (!prop_hle) {
        return KErrBadHandle;
    }

    kern->unsubscribe_prop(std::make_pair(prop_hle->first, prop_hle->second));

    return KErrNone;
}

BRIDGE_FUNC(TInt, RPropertyDefineNoSec, eka2l1::ptr<RProperty> aProp, TUid aCategory, TUint aKey, TInt aAttr, TInt aPreallocate) {
    memory_system *mem = sys->get_memory_system();
    kernel_system *kern = sys->get_kernel_system();

    RProperty *prop_lle = aProp.get(mem);

    property_ptr prop_hle = kern->create_prop(aAttr >= 0 ? service::property_type::bin_data : service::property_type::int_data,
        aPreallocate);

    prop_hle->first = aCategory;
    prop_hle->second = aKey;

    prop_lle->iHandle = prop_hle->unique_id();

    return KErrNone;
}

BRIDGE_FUNC(TInt, RPropertyGetInt, eka2l1::ptr<RProperty> aProp, eka2l1::ptr<TInt> aVal) {
    memory_system *mem = sys->get_memory_system();
    kernel_system *kern = sys->get_kernel_system();

    RProperty *prop_lle = aProp.get(mem);
    TInt *val_pass = aVal.get(mem);

    property_ptr prop_hle = kern->get_prop(prop_lle->iHandle);

    if (!prop_hle) {
        return KErrNotFound;
    }

    int val = prop_hle->get_int();

    if (val == -1) {
        return KErrArgument;
    }

    *val_pass = val;

    return KErrNone;
}

BRIDGE_FUNC(TInt, RPropertyGetGlobalInt, TUid aCagetory, TUint aKey, eka2l1::ptr<TInt> aValue) {
    memory_system *mem = sys->get_memory_system();
    kernel_system *kern = sys->get_kernel_system();

    TInt *val_pass = aValue.get(mem);

    property_ptr prop_hle = kern->get_prop(aCagetory, aKey);

    if (!prop_hle) {
        return KErrNotFound;
    }

    int val = prop_hle->get_int();

    if (val == -1) {
        return KErrArgument;
    }

    *val_pass = val;

    return KErrNone;
}

const eka2l1::hle::func_map prop_register_funcs = {
    BRIDGE_REGISTER(525391138, RPropertyAttach),
    BRIDGE_REGISTER(1292343699, RPropertyCancel),
    BRIDGE_REGISTER(2000262280, RPropertyDefineNoSec),
    BRIDGE_REGISTER(3651614895, RPropertyGetInt),
    BRIDGE_REGISTER(1693319139, RPropertyGetGlobalInt)
};