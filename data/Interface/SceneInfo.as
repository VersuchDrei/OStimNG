
class SceneInfo extends MovieClip
{
	/* STAGE ELEMENTS */
	public var packData:TextField;
	public var animNameData:TextField;
	public var genderMapData: TextField;

	public function SceneInfo()
	{
		super();
		// constructor code
		//this.packData.text = "AAAAAA";
	}

	public function updateInfo(packVal:String, animNameVal:String, genderMapVal: String):Void
	{
		this.packData.text = packVal;
		this.animNameData.text = animNameVal;
		this.genderMapData.text = genderMapVal;
	}
}