import com.greensock.*;
import com.greensock.easing.*;
import gfx.io.GameDelegate;
import gfx.managers.FocusHandler;
import gfx.ui.InputDetails;
import gfx.ui.NavigationCode;
import Shared.GlobalFunc;
import skse;

// by osmosis-wrench 2022
class asf_menu extends MovieClip
{
	var OptionsContainer_MC: MovieClip;
	var SpeedControl_MC: MovieClip;
	var CurrentAnimInfo_MC: MovieClip;
	
	public function asf_menu() {
		super();
		trace("start");
	}
	
	function onLoad(): Void {
		trace("loaded");
	}
}