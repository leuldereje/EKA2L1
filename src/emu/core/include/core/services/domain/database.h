#pragma once

#include <core/services/domain/defs.h>

#include <cstdint>
#include <vector>

namespace eka2l1 {
    namespace service::database {
        struct domain {
            uint16_t id;
            uint16_t own_id;

            std::string descriptor;

            int init_state;
            int trans_timeout;
        };

        struct hierarchy {
            uint8_t id;
            std::string descriptor;

            std::vector<domain> domains;

            TDmTraverseDirection positive_dir;
            TDmTraverseDirection neg_dir;

            TDmTranstitionFailurePolicy fail_policy;
        };

        constexpr uint32_t hierarchy_power_id = 0x1;
        constexpr uint32_t hierarchy_startup_id = 0x2;

        constexpr uint32_t kms2_hal_domain_3 = 30;
        constexpr uint32_t kms2_kernel_domain_3 = 80;
        constexpr uint32_t kms2_os_services_domain_3 = 130;
        constexpr uint32_t kms2_gen_middleware_domain_3 = 180;
        constexpr uint32_t kms2_gen_middleware_domain_4 = 190;
        constexpr uint32_t kms2_app_services_domain_3 = 230;
        constexpr uint32_t kms2_app_services_domain_4 = 240;
        constexpr uint32_t kms2_ui_services_domain_3 = 280;
        constexpr uint32_t kms2_ui_apps_domain_3 = 330;

        constexpr uint32_t power_root = 0x1;
        constexpr uint32_t apps = 0x2;
        constexpr uint32_t apps_ui = 0x3;

        constexpr uint32_t power_hierarchy_trans_timeout = 100000;
        constexpr uint32_t startup_hierarchy_trans_timeout = 10000000;

        // clang-format off
        const static hierarchy power_hierarchy = {
            hierarchy_power_id, "PowerHierarchy", { 
                { power_root, 0, "PowerRoot", ESsmStartupSubStateCriticalStatic, power_hierarchy_trans_timeout }, 
                { apps, power_root, "Apps", ESsmStartupSubStateCriticalStatic, power_hierarchy_trans_timeout },
                { apps_ui, apps, "AppsUi", ESsmStartupSubStateCriticalStatic, power_hierarchy_trans_timeout }
            }, ETraverseChildrenFirst, ETraverseParentFirst, ETransitionFailureContinue
        };

        const static hierarchy startup_hierarchy = {
            hierarchy_startup_id, "StartupHierarchy", { 
                { kms2_hal_domain_3, 0, "Kms2HALDomain3", ESsmStartupSubStateCriticalStatic, startup_hierarchy_trans_timeout }, 
                { kms2_kernel_domain_3, kms2_hal_domain_3, "Kms2KernelDomain3", ESsmStartupSubStateCriticalStatic, startup_hierarchy_trans_timeout }, 
                { kms2_os_services_domain_3, kms2_kernel_domain_3, "Kms2OsServicces3", ESsmStartupSubStateCriticalStatic, startup_hierarchy_trans_timeout }, 
                { kms2_gen_middleware_domain_3, kms2_os_services_domain_3, "Kms2GenMiddlewareDomain3", ESsmStartupSubStateCriticalStatic, startup_hierarchy_trans_timeout },
                { kms2_gen_middleware_domain_4, kms2_gen_middleware_domain_3, "Kms2GenMiddlewareDomain4", ESsmStartupSubStateCriticalStatic, startup_hierarchy_trans_timeout },
                { kms2_app_services_domain_3, kms2_gen_middleware_domain_4, "Kms2AppServicesDomain3", ESsmStartupSubStateCriticalStatic, startup_hierarchy_trans_timeout }, 
                { kms2_app_services_domain_4, kms2_app_services_domain_3, "Kms2AppServicesDomain4", ESsmStartupSubStateCriticalStatic, startup_hierarchy_trans_timeout }, 
                { kms2_ui_services_domain_3, kms2_app_services_domain_4, "Kms2UiServicesDomain3", ESsmStartupSubStateCriticalStatic, startup_hierarchy_trans_timeout },
                { kms2_ui_apps_domain_3, kms2_ui_services_domain_3, "Kms2UiServicesDomain3", ESsmStartupSubStateCriticalStatic, startup_hierarchy_trans_timeout } 
            }, ETraverseParentFirst, ETraverseParentFirst, ETransitionFailureContinue
        };

        const static std::vector<hierarchy> hierarchies_db = {
            power_hierarchy,
            startup_hierarchy
        };

        // clang-format on
    }
}