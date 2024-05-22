#include "GameVariableDropDownSetting.h"

namespace Settings {
    namespace GameVariableSettings {
        GameVariableDropDownSetting::GameVariableDropDownSetting(BaseTypes::CommonSettingParams params, BaseTypes::DropDownSettingParams dropDownParams, GameAPI::GameVariable variable)
            : BaseTypes::DropDownSetting(params, dropDownParams),
              variable{variable} {}

        dropDownIndex GameVariableDropDownSetting::getCurrentIndex() {
            return static_cast<int>(variable.getValue());
        }

        bool GameVariableDropDownSetting::setIndex(dropDownIndex index) {
            variable.setValue(index);
            return shouldRedraw();
        }
    }
}