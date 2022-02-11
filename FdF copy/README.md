# my_fdf

My fdf logic how to draw a line

FdF is  interesting 3D grapics project. It is helpful to divide this big pie into small pieces.

~ step 1: main (and project) structure
    - check ./fdf usage: ac == 2
    - open map  
    - create map  
    - get map;
    - draw map;


~ step 2: parse & store
    - fn: open, get_map, get_xy, get_z
    - parse the data and check if you have a valid map.
    - you can make the checks during parsing: 
            - usage: ac == 2
            - reading: open > 0
            - valid map: same width in every line
            - create map: memory err
            

    - go with get_next_line line by line, get the width and the height of the map, check if the number of points in every line is the same (valid map), store x & y coords, store z coords and color. 
    - get and store max & min z values, to create the color range in the future.

~ step 3: draw map
    - 


Bug fixing sometimes can be very entertaining:

()