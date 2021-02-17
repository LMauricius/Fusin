#include "Utilities/FusinConfigMap.h"

namespace Fusin
{

    template<>
    bool getCfgValue<bool>(const std::map<String, String>& config, String key, const bool def)
    {
		std::map<String, String>::const_iterator cfgIt = config.find(key);
		StringStream ss;

        if (cfgIt != config.cend())
            return (
                cfgIt->second == FUSIN_STR("True") ||
                cfgIt->second == FUSIN_STR("true") || 
                cfgIt->second == FUSIN_STR("1")
                );
        else
            return def;
    }
}