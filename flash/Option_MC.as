// by osmosis-wrench 2022
import com.greensock.*;

class Option_MC extends MovieClip
{
	var _w: Number;
	var _h: Number;
	var myIdx: Number;
	
	var NodeID: Number;
  	var Title: String;
  	var ImagePath: String;
  	var Description: String;
	var NodeInfoBox: MovieClip;
	var optionText: TextField;
	var TextureLoader; 
	var textureContainer :MovieClip;
	
	public function Option_MC()
	{
		super();
		_w = this._width;
		_h = this._height;
		this._alpha = 80;
		NodeInfoBox = _parent._parent.NodeInfoBox;
		HideOption();
		TextureLoader = new MovieClipLoader();
			
	}
	
	// Public Functions
	public function OnHighlight()
	{
		//_parent.CurrentlyHighlightedIdx = myIdx;
		if (_parent.CurrentlyHighlightedIdx != myIdx){
			_parent.Options[_parent.CurrentlyHighlightedIdx].OnUnHighlight();
			_parent.CurrentlyHighlightedIdx = myIdx;
		}
		NodeInfoBox.AssignData({Title: Title, Description: Description});
		this._width = _w;
		this._height = _h;
		this._alpha = 100;
		TweenLite.to(this, 0.5, {_width:_w+3, _height:_h+3});
	}
	
	public function OnUnHighlight()
	{
		this._alpha = 70;
		TweenLite.to(this, 0.2, {_width:_w, _height:_h});
	}
	
	public function OnSelect()
	{
		TweenLite.to(this, 0.5, {_width:_w+7, _height:_h+7});
		_parent.SendTransitionRequest(NodeID);
	}
	
	public function SetData(Node:Object)
	{
		NodeID = Node["NodeID"];
        Title = Node["Title"];
        ImagePath = Node["ImagePath"];
        Description = Node["Description"];
		optionText.text = String(Title);
		this.TextureLoader.loadClip(ImagePath, this.textureContainer);
		this.TextureLoader.addListener(this);
		NodeID != null ? this.ShowOption() : this.HideOption();
	}
	
	public function ShowOption()
	{
		this.enabled = true;
		this._alpha = 70;
	}
	
	public function HideOption()
	{
		this.enabled = false;
		this._alpha = 0;
	}
	
	function onLoadInit(aTargetClip)
	{
		if (undefined != this.TextureLoader && aTargetClip == this.textureContainer) 
		{
			if (undefined != this.textureContainer) 
			{
				this.textureContainer._width = 82;
				this.textureContainer._height = 82;
			}
			this.TextureLoader.removeListener(this);
			this.TextureLoader = undefined;
		}
	}
	// Private Functions
	
}