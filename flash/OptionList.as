import com.greensock.*;
import com.greensock.easing.*;
import gfx.io.GameDelegate;
import gfx.managers.FocusHandler;
import gfx.ui.InputDetails;
import gfx.ui.NavigationCode;
import Shared.GlobalFunc;
import skse;

// by osmosis-wrench 2022
class OptionList extends MovieClip
{
	var Option0: MovieClip;
	var Option1: MovieClip;
	var Option2: MovieClip;
	var Option3: MovieClip;
	var Option4: MovieClip;
	var Option5: MovieClip;
	var Option6: MovieClip;
	
	var Options: Array = new Array(7);
	
	public function OptionList() {
		super();
		Options = [Option0 , Option1, Option2, Option3, Option4, Option5, Option6 ]
	}
}

