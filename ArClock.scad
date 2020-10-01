/*
 * ArClock
 *
 * (c) Matt Aubury 2020
 */
 
/*
 * This design is somewhat parameterisable, but I wouldn't
 * bet that any given combination of parameters will result
 * in something that is printable and/or fits together.
 */
 
 /*
  * Number of LEDs in the gird
  */
grid_x = 32;
grid_y = 8;

/*
 * Spacing between LEDs, in mm
 */
spacing = 10;

/*
 * Overall thickness of the design, in mm. I printed mine
 * at 16mm, which is a little tight to fit the ESP8266 in,
 * so I've adapted it to 18mm here which should be sufficient.
 *
 * Increasing it further should be fairly harmless.
 */
thickness = 18; 

/*
 * Fudge factor for making the end cap small enough to slid 
 * into the main body
 */
epsilon = 0.1;

/*
 * Wall thickness, in mm. This can probably be reduced a
 * little (perhaps to 1.6) without too many problems, but
 * I like the rigidity of 2mm walls.
 */
wall_thickness = 2;

/*
 * Radius of the curve. Increasing this results in a flatter
 * profile (but may not fit even in a 300mm bed). Decreasing
 * it results in a more dramatic curve.
 *
 * If reducing it significantly I'd suggest increasing the 
 * thickness parameter a little to allow more space for the 
 * ESP8266.
 *
 * You will also need to tweak the mounting holes as these 
 * are not properly parameterised.
 *
 * 200 works for Prusa i3
 * 190 works for Ender 3 (with brim)
 * 500 works for CR10
 */
radius = 190;

/*
 * Size of the Wemos D1 mini, used to parameterise the mounting
 * plate. If you plan to use a different board you will need
 * to tweak these, but ALSO tweak the layout of the mounting
 * holes (which are not parameterised here).
 */
wemos_width = 25.4;
wemos_height = 34;

/*
 * Smoothness of the curve. At 500 the steps are essentially
 * imperceptible, but it takes a long time to render the 
 * design, so a smaller value can be used when prototyping.
 */
$fn=500;

/************************************************************/

/*
 * Calculated values
 */
total_x = grid_x * spacing + wall_thickness * 4;
total_y = grid_y * spacing + wall_thickness * 2;
outer_angle = 360 * (total_x / (radius - wall_thickness) / (2 * PI));
inter_angle = 360 * ((total_x - (wall_thickness * 2)) / (radius - wall_thickness) / (2 * PI));
inner_angle = 360 * ((grid_x * spacing) / (radius - wall_thickness) / (2 * PI));
angle_offset = (outer_angle - inter_angle) / 2;
angle_offset2 = (outer_angle - inner_angle) / 2;
epsilon_angle = 360 * (epsilon / (radius - wall_thickness) / (2 * PI));
wemos_angle = 360 * (wemos_width / (radius - (2.5 + wall_thickness * 2)) / (2 * PI));
wemos_offset = (outer_angle * 2) / 3;
notch_angle = 360 * (25 / (radius - (2.5 + wall_thickness * 2)) / (2 * PI));

/************************************************************/

/*
 * Center the overall design. This design should be rendered twice, 
 * once for the body and once for the end cap.
 */
translate ([-radius * cos(outer_angle/2), -radius * sin(outer_angle/2), 0])
{
    body();
    base ();
}

/************************************************************/

/*
 * Main body
 */
module body () 
{
    difference ()
    {
        outer_body ();
        union ()
        {
            innards ();
            grid_holes ();
            power_connector_hole ();
        }
    }
}

/*
 * Outer part of the body
 */
module outer_body ()
{
    difference ()
    {
        intersection ()
        {
            difference ()
            {
                cylinder (h=total_y, r=radius);
                translate ([0, 0, -epsilon])
                {
                    cylinder (h=total_y + epsilon * 2, r=radius - thickness);
                }
            }
            cube_segment ();
        }

        rotate ([0, 0, outer_angle])
        {
            cube_segment ();
        }
    }
}
/*
 * Innards of the body (NB: this is subtracted)
 */
module innards ()
{
    difference ()
    {
        intersection ()
        {
            difference ()
            {
                translate ([0, 0, wall_thickness])
                {
                    difference ()
                    {
                        cylinder (h=total_y, r=radius - wall_thickness);
                        cylinder (h=total_y, r=radius - (thickness - wall_thickness));
                    }
                }
                
                difference ()
                {
                    difference ()
                    {
                        cylinder (h=total_y - 20, r=radius - (wall_thickness + 2.5));
                        cylinder (h=total_y - 20, r=radius - (wall_thickness + 4.5));
                    }
                    
                    translate ([0, 0, 20])
                    {
                        difference ()
                        {
                            rotate ([0, 0, notch_angle])
                            {
                                cube_segment ();
                            }
                            rotate ([0, 0, outer_angle - notch_angle])
                            {
                                cube_segment ();
                            }
                        }
                    }
                }
            }
            rotate ([0, 0, angle_offset])
            {
                cube_segment ();
            }
        }

        rotate ([0, 0, outer_angle - angle_offset])
        {
            cube_segment ();
        }
    }
}

/*
 * Grid of holes for the LEDs
 */
module grid_holes ()
{
    for (j = [0 : grid_y - 1])
    {
        translate ([0, 0, j * spacing])
        {
            for (i = [0 : grid_x - 1])
            {
                rotate ([0, 0, angle_offset2 + (i + 0.5) * (inner_angle / grid_x)])
                {
                    translate ([radius, 0, (spacing / 2) + wall_thickness])
                    {
                        rotate ([0, 90, 0]) { cylinder (wall_thickness * 3, r=4, center=true, $fn=24); }
                    }
                }
            }
        }
    }
}

/*
 * External hole for power connector
 */
module power_connector_hole ()
{
    translate ([0, 0, total_y - 10])
    {
        rotate ([0, 90, outer_angle/2])
        {
            cylinder (h=radius - (thickness - wall_thickness * 2), r=4, $fn=48);
        }
    }
}

/************************************************************/

module base ()
{
    difference ()
    {
        intersection ()
        {
            union ()
            {
                bottom ();
                support ();
                mounting_plate ();
                mounting_holes ();
            }
            
            rotate ([0, 0, angle_offset + epsilon_angle])
            {
                cube_segment ();
            }
        }

        rotate ([0, 0, outer_angle - (angle_offset + epsilon_angle)])
        {
            cube_segment ();
        }
        
        rotate ([0, 0, wemos_offset])
        {
            translate ([radius - (2.5 + 9 + (wall_thickness * 2)), -25/2, wall_thickness-1])
            {
                /*
                 * USB hole
                 */
                translate ([6.5-3, 4.5 + 1.5 + 7 - 0.5, 0])
                {
                    cube ([4, 11, 20], center=true);
                }
            
                /*
                 * Case thinning for PCB
                 */
                translate ([4, 0, 0])              
                {
                    cube([3, wemos_width, 2]);
                }
            
                /*
                 * Through holes for mounting screws
                 */
                translate ([0, 0, 0.5])
                {
                    rotate ([0, 90, 0]) 
                    {
                        translate ([-3, 3, 0])
                        cylinder (r=1, h=radius*2, center=false);
                    }

                    rotate ([0, 90, 0]) 
                    {
                        translate ([3-wemos_height, wemos_width-2.54, 0])
                        cylinder (r=1, h=radius*2, center=false);
                    }
                }
            }
        }
    }    
}

/*
 * The flat bottom
 */
module bottom ()
{
    difference ()
    {
        cylinder (h=wall_thickness, r=radius - (wall_thickness + epsilon));
        cylinder (h=wall_thickness, r=radius - (thickness - (wall_thickness + epsilon)));
    }
}

/*
 * The support holding the LED matrix in place
 */
module support ()
{
    difference ()
    {
        difference ()
        {
            cylinder (h=20 - epsilon, r=radius - (wall_thickness + 2.5));
            cylinder (h=20 - epsilon, r=radius - (wall_thickness + 4.5));
        }

        /*
         * Notch for power connector
         */
        translate ([0, 0, wall_thickness * 2])
        {
            difference ()
            {
                rotate ([0, 0, (outer_angle - notch_angle)/2])
                {
                    cube_segment ();
                }
                rotate ([0, 0, (outer_angle + notch_angle)/2])
                {
                    cube_segment ();
                }
            }
        }
    }
}

/*
 * WEMOS mounting plate
 */
module mounting_plate ()
{
    intersection ()
    {
        difference ()
        {
            cylinder (h=wemos_height + wall_thickness, r=radius - (wall_thickness + 2.5));
            cylinder (h=wemos_height + wall_thickness, r=radius - (wall_thickness + 4.5));
        }
        
        difference ()
        {
            rotate ([0, 0, wemos_offset - (wemos_angle/2)])
            {
                cube_segment ();
            }
            rotate ([0, 0, wemos_offset + (wemos_angle/2)])
            {
                cube_segment ();
            }
        }
    }                
}

/*
 * Through holes for mounting the WEMOS
 */
module mounting_holes ()
{
    rotate ([0, 0, wemos_offset])
    {
        translate ([radius - (2.5 + 9 + (wall_thickness * 2)), -25/2, wall_thickness - 0.5])
        {
            %cube([9, wemos_width, wemos_height]);    
            
            rotate ([0, 90, 0]) 
            {
                translate ([-3, 3, thickness-11])
                difference ()
                {
                    cylinder (r=2.5, h=3, center=false);
                    cylinder (r=.8, h=3, center=false);
                }
            }

            rotate ([0, 90, 0]) 
            {
                translate ([3-wemos_height, wemos_width-2.54, thickness-11])
                difference ()
                {
                    cylinder (r=2.5, h=3, center=false);
                    cylinder (r=.8, h=3, center=false);
                }
            }
        }
    }
}

/************************************************************/

/*
 * Cube used to slice out radial segments
 */
module cube_segment ()
{
    translate ([-radius, 0, 0])
    {
        cube ([radius * 2, radius * 2, total_y]);
    }
}

/************************************************************/
