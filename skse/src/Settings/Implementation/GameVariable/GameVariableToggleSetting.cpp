#include "GameVariableToggleSetting.h"

namespace Settings {
    namespace GameVariableSettings {
        GameVariableToggleSetting::GameVariableToggleSetting(BaseTypes::CommonSettingParams params, BaseTypes::ToggleSettingParams toggleParams, GameAPI::GameVariable variable)
            : BaseTypes::ToggleSetting(params, toggleParams),
              variable{variable} {}

        bool GameVariableToggleSetting::isActivated() {
            return variable.getValue() != 0.0f;
        }

        bool GameVariableToggleSetting::toggle() {
            if (variable.getValue() == 0.0f) {
                variable.setValue(1.0f);
            } else {
                variable.setValue(0.0f);
            }

            return shouldRedraw();
        }
    }
}