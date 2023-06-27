import com.greensock.*;

class SearchOption extends MovieClip
{
	var searchOptionBg : MovieClip;
	var idx = 0;
	public function SearchOption()
	{
		// constructor code
	}
	
	function handleInput(details, pathToFocus){
		
	}
	
	function OnHighlight(){
		TweenLite.to(searchOptionBg,0.5,{alpha:70});
		searchOptionBg._visible = true;
	}
	
	function OnUnHighlight(){
		TweenLite.to(searchOptionBg,0.2,{alpha:00});
		searchOptionBg._visible = false;
	}
}