
class InfoField extends MovieClip
{

	private static var GREEN: Number = 0x009900;
	private static var WHITE: Number = 0xFFFFFF;

	public var offsetData:TextField;
	public var upButton: MovieClip;
	public var downButton: MovieClip;
	
	public function InfoField()
	{
		// constructor code
		upButton.stop();
		downButton.stop();
	}

	public function updateText(textVal:String):Void
	{
		offsetData.text = textVal;
	}

	public function select():Void
	{
		offsetData.textColor = GREEN;
	}

	public function deselect():Void
	{
		offsetData.textColor = WHITE;
	}

	public function animateUpdateText(textVal:String, up :Boolean):Void
	{
		if(up){
			upButton.play();
		} else{
			downButton.play();
		}
		this.updateText(textVal);
	}
}