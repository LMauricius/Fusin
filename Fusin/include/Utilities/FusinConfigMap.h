#ifndef _FUSIN_CONFIG_MAP_H
#define _FUSIN_CONFIG_MAP_H

#include "FusinPrerequisites.h"
#include <map>

namespace Fusin
{
    template<class _T>
    _T getCfgValue(const std::map<String, String>& config, String key, const _T def)
    {
		std::map<String, String>::const_iterator cfgIt = config.find(key);
        if (cfgIt != config.cend())
        {
		    StringStream ss;
            ss << cfgIt->second;
            _T res;
            ss >> res;
            return res;
        }
        else
            return def;
    }

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

#endif