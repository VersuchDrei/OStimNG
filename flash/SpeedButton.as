class SpeedButton extends MovieClip
{

	var shape:MovieClip;

	public function Show(showButton:Boolean)
	{

		if (!showButton)
		{
			shape.gotoAndStop(31);
		}
		else
		{
			shape.gotoAndStop(1);
		}
	}
}