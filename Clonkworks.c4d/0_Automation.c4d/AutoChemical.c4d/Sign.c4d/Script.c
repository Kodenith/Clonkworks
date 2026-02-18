/*-- Neues Script --*/

#strict

private func ShadowID(){ return(1); }
private func MainID(){ return(2); }
func SetPic(FilterImage){
	//outline
  SetGraphics(0,this(),FilterImage,ShadowID(),4);
  SetObjDrawTransform(800,0,((GetDefCoreVal("Picture","DefCore",FilterImage,2)/2)*900)/4,0,800,GetDefHeight(GetID())*1000/4+(((GetDefCoreVal("Picture","DefCore",FilterImage,3)/2)*1000)/4)-10000,this(),ShadowID());
  SetClrModulation(RGBa(0,0,1),this(),ShadowID());
  
  //main
  SetGraphics(0,this(),FilterImage,MainID(),4);
  SetObjDrawTransform(700,0,((GetDefCoreVal("Picture","DefCore",FilterImage,2)/2)*900)/4,0,700,GetDefHeight(GetID())*1000/4+(((GetDefCoreVal("Picture","DefCore",FilterImage,2)/2)*1000)/4)-10000,this(),MainID());
}
