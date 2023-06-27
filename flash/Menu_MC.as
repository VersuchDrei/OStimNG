// by osmosis-wrench 2022
import gfx.managers.FocusHandler;
import gfx.ui.InputDetails;
import gfx.ui.NavigationCode;
import Shared.GlobalFunc;

class Menu_MC extends MovieClip
{
	var OptionBox: MovieClip;
	var InfoBox: MovieClip;
	
	public function Menu_MC()
	{
		super();
		FocusHandler.instance.setFocus(this,0);
		
		// uncomment to test in flash.
/*		var keyListener:Object = new Object();
		keyListener.onKeyDown = function() {
			if(Key.getCode() == Key.UP){
				//do something here for space bar press
				_root.Menu.OptionBox.HandleKeyboardInput(0);
			}
			if(Key.getCode() == Key.DOWN){
				//do something here for space bar press
				_root.Menu.OptionBox.HandleKeyboardInput(1);
			}
			if(Key.getCode() == Key.LEFT){
				//do something here for space bar press
				_root.Menu.OptionBox.HandleKeyboardInput(2);
			}
			if(Key.getCode() == Key.RIGHT){
				//do something here for space bar press
				_root.Menu.OptionBox.HandleKeyboardInput(3);
			}
			if(Key.getCode() == Key.SPACE){
				//do something here for space bar press
				_root.Menu.OptionBox.HandleKeyboardInput(4);
			}
		};
		Key.addListener(keyListener);*/
	}
	
	public function onLoad(){
		var s = SpoofData();
		ReceiveData(s[0], s[1][0], s[1][1], s[1][2], s[1][3], s[1][4], s[1][5], s[1][6], s[1][7], s[1][8], s[1][9], s[1][10], s[1][11]);
	}
	
	function handleInput(details:InputDetails, pathToFocus:Array):Void
	{
		//_root.test1.text = "value=" + details.value+ "navEquivalent=" + details.navEquivalent;
		if (GlobalFunc.IsKeyPressed(details)){
			if (details.navEquivalent == NavigationCode.UP){
				OptionBox.HandleKeyboardInput(0);
			} else if (details.navEquivalent == NavigationCode.DOWN){
				OptionBox.HandleKeyboardInput(1);
			} else if (details.navEquivalent == NavigationCode.LEFT){
				OptionBox.HandleKeyboardInput(2);
			} else if (details.navEquivalent == NavigationCode.RIGHT){
				OptionBox.HandleKeyboardInput(3);
			} else if (details.navEquivalent == NavigationCode.ENTER){
				OptionBox.HandleKeyboardInput(4);
			}
		}
	}
	
	public function SpoofData():Object
	{
		var Arg0:Object = new Object();
		Arg0["NodeID"] = "Main";
		Arg0["Title"] = "Test Animation Name";
		Arg0["CurrentSpeed"] = 1;
		Arg0["MaxSpeed"] = 5;
		Arg0["Animator"] = "Main Animator";
		
		var ArgN:Object = new Object();
		for (var i = 0; i < 12; i++){
			var o:Object = new Object();
			o["NodeID"] = i;
			o["Title"] = "Title "+i;
			o["ImagePath"] = "NA";
			o["Description"] = "The description for Title "+i;
			o["Animator"] = "Animator"+i;
			//if(i >= 5){
				//o["NodeID"] = null;
			//}
			ArgN[i] = o;
		}
		var i;
		return [Arg0, ArgN]
	}
	
	public function ReceiveData()
	{
		trace(arguments.length);
		var EdgeData:Object = new Object();
		for (var i = 1; i < arguments.length; i++){
			EdgeData[i] = arguments[i];
		}
		OptionBox.AssignData(EdgeData);
		InfoBox.AssignData(arguments[0]);
	}
}