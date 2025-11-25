/*-- Neues Script --*/

#strict 2

//Returns a random color
//Include Alpha - Alpha is also random
global func RandomRGBa(bool IncludeAlpha){
	if(IncludeAlpha) return(RGBa( RandomX(0,255) , RandomX(0,255) , RandomX(0,255) , RandomX(0,255)));
	return(RGBa( RandomX(0,255) , RandomX(0,255) , RandomX(0,255)));
}

//mixes two different colors.
global func MixColor(int col1, int col2){
	var bi = 2;
	var R,G,B,R2,G2,B2,foo;
	SplitRGBaValue(col1,R,G,B,foo);
	SplitRGBaValue(col2,R2,G2,B2,foo);
	
	var endR = bi*R+R2*(5-bi);
	var endG = bi*G+G2*(5-bi);
	var endB = bi*B+B2*(5-bi);
	var scale = Max(Max(endR,endG),endB)/((2*bi*Max(Max(R,G),B)+(2-2*bi)*Max(Max(R2,G2),B2))/2);
	endR = endR/scale;
    endG = endG/scale;
    endB = endB/scale;

/* 	endR = BoundBy(endR,0,255);
	endG = BoundBy(endG,0,255);
	endB = BoundBy(endB,0,255); */
	
	return(RGBa(endR,endG,endB));
}

//loosens any material, unless its not blastable
//made this for the heat ray, as ShakeFree only breaks diggable materials.
global func Loosen(x , y , iRadius)
{
	var iRad = Abs(iRadius);
	if(GetType(x) == C4V_Any) x = 0;
	if(GetType(y) == C4V_Any) y = 0;
	
	var mateusz = MaterialName(GetMaterial(x,y));
	if(GetMaterialVal("BlastFree", "Material", GetMaterial(x, y))){
	BlastFree(x,y,1);
	if(!GBackSolid(x,y)){
		CastPXS(mateusz,1,10,x,y);
	}
	}

	for(var z = iRad; z > 0; z --)
	{
		for(var i = 0; i < 360; i ++)
		{
			if(GBackSolid(Cos(i , z) + x , Sin(i , z) + y)){
				var mat = MaterialName(GetMaterial(Cos(i , z) + x,Sin(i , z) + y));
				if(GetMaterialVal("BlastFree", "Material", GetMaterial(Cos(i , z) + x,Sin(i , z) + y))){
				BlastFree(Cos(i , z) + x,Sin(i , z) + y,1);
				if(!GBackSolid(Cos(i , z) + x , Sin(i , z) + y)){
					CastPXS(mat,1,10,Cos(i , z) + x,Sin(i , z) + y);
				}
				}
			}
		}
	}

	return true;
}

//get the highest container if an object is contained
global func ContainedTop(pObj){
	if(!Contained(pObj)) return(0);
	while(true){
		if(Contained(pObj)) pObj = Contained(pObj);
		else return(pObj);
	}
}