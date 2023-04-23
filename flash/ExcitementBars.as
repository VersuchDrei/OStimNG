
class ExcitementBars extends MovieClip
{
	var bars : Array;
	var testText : MovieClip;	
	public function ExcitementBars(){
		// constructor code
	}
	
	public function Update(values : Array){
		for(var i in values){
			//bars[i].Update(val);
		}
	}
	
	public function AssignData(barsData){
		testText.text = "AAAAA";
		cleanUp();
		trace(barsData.actors.length);
		for(var i in barsData.actors){
			var id = "excBar" + Number(i);
			bars[i] = this.createEmptyMovieClip(id, this.getNextHighestDepth());
			loadMovie("OstimBar.swf", bars[i]);
			bars[i]._y = (i * -50);
			bars[i].visible = true;
		}
	}
	
	public function cleanUp(){
		
	}
}