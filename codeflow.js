/* implementations of code algorithms
   --from eric's last year's osu final project lessons:
   ----> vector graphics: spellcards implement y=x^3 movement
   ------>bullets implement different functions
   ---------1)sin/cos
   ---------2)homing-tracking
   ---------3)bezier curves?
   ---------4)circular paths deviating from a point
   ------------>(incrementing normal and angular acceleration

   ---->data structures: lookup tables again ( for bezier )
   ---->

   --from Barak's experience:
   ----> Screw up memory/object management, screw up life
   ------> Make everything flexible
   ------> Use states
*/
//all objs that can be drawn in the GUI
var obj={
    hp:depends,
    speed:depends,
    x:depends,
    y:depends,
    var character={
	hp:1,//seriously this is a hit and die game
	lives:3,
	bombs:3
    }
    var mob = {
	//regular monster
	is_boss:bool;
	norm_attack(){},
	var list_of_skills = {
	    spell_1(){},
	    spell_n(){}
	}
    }
    var projectile = {
	
    }
    var bullet = {
    }
}
