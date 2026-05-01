#strict 2

func Initialize(){
  ScheduleCall(this,"Redefine",1);
}

func Redefine(){

  var real;
  //SLog(Contained());
  if(!Contained()){
      real = CreateObject(CLBT,0,10,GetOwner());
  }else{
    real = CreateContents(CLBT,Contained(),1);
    //SLog(real);
    SetCommand(real,"Exit");
  }
  return(RemoveObject());
}