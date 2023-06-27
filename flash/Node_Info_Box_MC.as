// by osmosis-wrench 2022

class Node_Info_Box_MC extends MovieClip
{
	var Animation_Name_Text: TextField;
	var Animation_Description_Text: TextField;
	
	public function Node_Info_Box_MC()
	{
		super();
	}
	
	function AssignData(Current:Object)
	{
		Animation_Name_Text.text = Current["Title"];
		Animation_Description_Text.text = Current["Description"];
	}
}
