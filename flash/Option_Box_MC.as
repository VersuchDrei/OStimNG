// by osmosis-wrench 2022
import skse;
import gfx.managers.FocusHandler;
import Mouse;

class Option_Box_MC extends MovieClip
{
	var o0 : MovieClip;
	var o1 : MovieClip;
	var o2 : MovieClip;
	var o3 : MovieClip;
	var o4 : MovieClip;
	var o5 : MovieClip;
	var o6 : MovieClip;
	var o7 : MovieClip;
	var o8 : MovieClip;
	var o9 : MovieClip;
	var o10 : MovieClip;
	var o11 : MovieClip; 
	
	var Options: Array = new Array(12);
	var CurrentlyHighlightedIdx: Number;
	
	public function Option_Box_MC()
	{
		super();
		CurrentlyHighlightedIdx = -1;
		Options = [o0, o1, o2, o3, o4, o5, o6, o7, o8 ,o9 , o10, o11];
		
		for (var i in Options){
			Options[i].myIdx = Number(i);
			Options[i].onRollOver = function(){
				this.OnHighlight();
			};
			Options[i].onRollOut = function(){
				this.OnUnHighlight();
			};
			Options[i].onMouseDown = function(){
				//trace(_parent);
				if (Mouse.getTopMostEntity()._parent == this){
					this.OnSelect();
				}
			};
		}
	}
	
	public function HandleKeyboardInput(e:Number)
	{
		// 0 = up, 1 = down, 2 = left, 3 = right, 4 = select;
		// Sanitize starting idx
		trace("Handle "+CurrentlyHighlightedIdx);
		if (CurrentlyHighlightedIdx < 0) {
			CurrentlyHighlightedIdx = 0;
			Options[CurrentlyHighlightedIdx].OnHighlight();
		} else {
			Options[CurrentlyHighlightedIdx].OnUnHighlight();
			trace("inner "+CurrentlyHighlightedIdx);
			switch(e){
				case 0:
					trace("up");
					CurrentlyHighlightedIdx + 3 > 11 ? CurrentlyHighlightedIdx = CurrentlyHighlightedIdx - 9 : CurrentlyHighlightedIdx = CurrentlyHighlightedIdx + 3;
					break;
				case 1:
					trace("down");
					CurrentlyHighlightedIdx - 3 < 0 ? CurrentlyHighlightedIdx = CurrentlyHighlightedIdx + 9 : CurrentlyHighlightedIdx = CurrentlyHighlightedIdx - 3;
					break;
				case 2:
					trace("left");
					CurrentlyHighlightedIdx = CurrentlyHighlightedIdx - 1;
					break;
				case 3:
					trace("right");
					CurrentlyHighlightedIdx = CurrentlyHighlightedIdx + 1;
					break;
				case 4:
					trace("select");
					Options[CurrentlyHighlightedIdx].OnSelect();
					break;
			}
		}
		CurrentlyHighlightedIdx > 11 ? CurrentlyHighlightedIdx = 0 : null;
		CurrentlyHighlightedIdx < 0 ? CurrentlyHighlightedIdx = 11 : null;
		Options[CurrentlyHighlightedIdx].OnHighlight();
		trace("Out "+CurrentlyHighlightedIdx);
		trace("---");
	}
	
	
	public function SendTransitionRequest(NodeID:Number)
	{
		log("UI_TransitionRequest {}, {"+NodeID+"}");
		skse.SendModEvent("UI_TransitionRequest", null, NodeID);
	}
	
	public function SendSpeedRequest(SpeedIdx:Number)
	{
		log("UI_SpeedRequest {}, {"+SpeedIdx+"}");
		skse.SendModEvent("UI_SpeedRequest", null, SpeedIdx);
	}
	
	public function AssignData(Edges:Object)
	{
		var i;
		for(i in Edges){
			Options[i-1].SetData(Edges[i]);
		}
	}
	
	function log(str:String)
	{
		_root.test1.text = str;
		trace(str);
	}
}