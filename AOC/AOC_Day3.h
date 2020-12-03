#pragma once

char const* testInput[] = {
"..##.......",
"#...#...#..",
".#....#..#.",
"..#.#...#.#",
".#...##..#.",
"..#.##.....",
".#.#.#....#",
".#........#",
"#.##...#...",
"#...##....#",
".#..#...#.#",
};


char const* part1Input[]{
".....##.#.....#........#....##.",
"....#...#...#.#.......#........",
".....##.#......#.......#.......",
"...##.........#...#............",
"........#...#.......#.........#",
"..........#......#..#....#....#",
"..................#..#..#....##",
".....##...#..#..#..#..#.##.....",
"..##.###....#.#.........#......",
"#.......#......#......#....##..",
".....#..#.#.......#......#.....",
"............#............#.....",
"...#.#........#........#.#.##.#",
".#..#...#.....#....##..........",
"##..........#...#...#..........",
"...........#...###...#.......##",
".#..#............#........#....",
"##.#..#.....#.......#.#.#......",
".##.....#....#.#.......#.##....",
"..##...........#.......#..##.#.",
"##...#.#........#..#...#...#..#",
".#..#........#.#.......#..#...#",
".##.##.##...#.#............##..",
"..#.#..###......#..#......#....",
".#..#..#.##.#.##.#.#...........",
"...#....#..#.#.#.........#..#..",
"......#.#....##.##......#......",
"#....#.##.##....#..#...........",
"...#.#.#.#..#.#..#.#..#.##.....",
"#.....#######.###.##.#.#.#.....",
"..#.##.....##......#...#.......",
"..#....#..#...##.#..#..#..#..#.",
".............#.##....#.........",
".#....#.##.....#...............",
".#............#....#...#.##....",
".#.....#.##.###.......#..#.....",
".#...#.........#.......#..#....",
"..#.#..#.##.......##...........",
".....##..#..#..#..#.##..#.....#",
"..##............##...#..#......",
"...#..#....#..##.....##..#.#...",
"#.....##....#.#.#...#...#..##.#",
"#.#..#.........#.##.#...#.#.#..",
".....#.#....##....#............",
"#.......#..#.....##..#...#...#.",
".....#.#...#...#..#......#.....",
"..##....#.#.#.#.#..#...........",
"##..#...#.........#......#...#.",
"..#...#.#.#.#..#.#.##..##......",
"#............###.....###.......",
"..........#...#........###.....",
".......##...#...#...#........#.",
".#..#.##.#.....................",
".#..##........##.##...#.......#",
".......##......#.....#......#..",
".##.#.....#......#......#......",
"#...##.#.#...#.#...............",
"........#..#...#.##.......#....",
"...................#...#...##..",
"...#...#.........#.....#..#.#..",
".###..#........#..##.##..#.##..",
"#...#.....#.....#.....#..#..#..",
"###..#.....#.#.#.#......#....#.",
"#........#....##.#...##........",
".#.#..##........##....##.#.#...",
"#...#....#.###.#.#.........#...",
"...#...##..###.......#.........",
"......#....#..##..#.....#.#....",
"........#...##...###......##...",
"..........##.#.......##........",
"...#....#......#...##.....#....",
"###.#.....#.#..#..#....#...#..#",
".#.....#.#....#...............#",
"..#....#....####....###....#.#.",
"....##........#..#.##.#....#...",
".......##...#...#..#....####...",
"#...##.#......##...#..#........",
"..##..#.##....#.......##.#.#...",
"..#.#...............#...#.#....",
"....#.....#.#.....#.##.......#.",
"...#.#..##.#.#..............##.",
"..#.....#...#.............#.##.",
"##..#.#...#........#..#.....##.",
"...........##...#.#.###...#....",
"...#.#.#..#..................#.",
".#...##.............#...#......",
"..#..#...#.#.......#...#.....#.",
"..##.......#.#.................",
".##..#........###.....#....#.##",
"......#..###.......#....##....#",
"....#.....#.................#..",
"........#...#...#..............",
"...#..#.###.......#..#.#.#.##..",
"..#...#.....#....#.........#...",
"...#.............#........###..",
"......#..............#......#..",
"#..#...........#...#..........#",
"...##...#.###..#...#.....#.#...",
"....#..##......#.......##......",
"....#....##.#...#.#..#....#...#",
".#...........#..#....##...#..##",
"..#.#.................###.#...#",
"..#.#.#...##...........#.......",
"..........#..##...#.#..##....##",
"........#........#.##..#.#...#.",
".....#...##.......##......#...#",
"....#...#..#..#.....#..........",
".#..#......#..#..#..###.......#",
".##..........#...#...#.#.....##",
"..#..........#.#.#...###.......",
"....#................#...##....",
".##..#....#..........#.#.#.....",
"..##...#.#........#.....#.##...",
"....####.....#..#.........##..#",
"......#.........#...#..........",
"....#...................#..##..",
".##....#.#.........#....#...#..",
"....##...##.....#..####........",
"..##.#....#.#.......##...#.....",
"#...#.#.#...#..#..##.....#.....",
"#..................###.....#...",
"#.#.....#.......#.#...###.#....",
".#..#....#............#........",
"#.#....#..#.#...............#..",
"..#..#..#.............#......#.",
"..#.......##...................",
".#....#.........#....#.#.#..#..",
"....#....#..#...............#..",
"......#..#..##......#.........#",
"..#.##........##......#..#..#.#",
"#.....#.#....#.........##...#..",
"###..............#....###...##.",
"....#..##......#.......##......",
"......#...#.##......##....#..#.",
"..........#....#..##.......#..#",
".#..#...##..#...........#..#..#",
".....#....#...#..###...###....#",
".#####..#...#.#.#..#.#.###...##",
"..##............##.#...#.##...#",
".##..#...#...#....##.#..#..##..",
".#....#...#............##..#...",
".#.#......#....#....#..##..##..",
".........#...#.......#.##..#...",
"#.........#.....##.....#..#..#.",
"...##.#...#...#..#..#....##..##",
".#............#...#....##......",
"..#...#.##.........#.#......#.#",
"....#.##........#.........#..##",
"#.........#......#.#......#..#.",
"........#.#.......#.#........#.",
"..#..........##.#...#..#.#.....",
"..#...#....#...#...#..#.#..#.#.",
".#.........#....#..#####..#....",
"#.#....#.#.###...#.............",
"..##...........##......##......",
"#.....#..#....#...............#",
"...#.#..#....##......#...##....",
"...#........#.....#...#..#.....",
".#......##.........#......#....",
"..#..###.##...#.#.....#........",
".............#......#..#.......",
"..#...............#.#...#..#..#",
".......#..#...#.#####......#..#",
".........#.....#...............",
"##........#............#.#.....",
".#...#.....#..#..#...#....#...#",
"..#....#....##......##.....#.#.",
"#...##..##......#...#....#.....",
"....#.#.#.....###....##.##....#",
"..........##...##.......#......",
"..#.......#...##.#....##.##....",
"....#........................#.",
"...#...#.#.##...#.....#...#..#.",
".#....##..#..#..........##..##.",
".#.....#..#...#.##.....#.......",
".#.##...#.#..#.....##....#...#.",
".##...#........##....#..#......",
".....#........#..........#.#..#",
"....#..##.......#..#.....#.....",
"...........#...#........#.##..#",
".....#..#....#..#.#.....#....##",
".....#....#.##.#..##...........",
"...##.......##.........#.......",
"...............##..#....#.#....",
".......###..#........#..####.##",
".......#.##...#.#....#.####....",
"....#...............#..........",
"##.#.......#.....#......#...#..",
"......##.....#....#.....#..#..#",
".....#...##.............#......",
"#.#.##.#.....#..#........#.....",
"......##....#..#........#......",
"............#........#..#.#....",
"##.......#......#...####..#.##.",
"..##..#...#.............#.##...",
".....#..##......#.##......###..",
"............#........#........#",
"#.#.#.#...#.#.....#.........#..",
".........#...............#.....",
".............###.#.......#....#",
"###.##..#..#..........#....#...",
"#......#...#..#..#.....#.##....",
"............#....#....#..#.....",
"..#.#....#...#......#.#..#..##.",
"...#........................#..",
"#.#...#..........#......#.#....",
".........#................#...#",
"##.....#....#........##.......#",
"#...##........#...#...........#",
"...#...#..........##.......#.#.",
"..#.#.#....#......##...........",
"...#.#...#.##.#..#.#.##........",
"#....##.....###..#.......#.....",
"###.....#.#.#...#..#.........##",
"..#......#..###...#.#.#.....#.#",
".#....#.....#............#..##.",
"....#....##..........#.....##..",
"#...........#....#...#..#...##.",
"..#.......#.....#..........#...",
".#..#................#......#..",
"..#......#.#...#..#.#....#....#",
"...#..#...###..#..##....#.#....",
"..#..............#.....#.......",
"...#.#...#.........#.#.........",
"##......##...........##.#.##..#",
"..#..##..#....#.#......#.#...##",
"...#.###....###...#.....#......",
"#.#................#......#....",
"..#.....#.....#....##.......#..",
".#.#...............##..#.......",
"...#....#.......#.#.....##..#..",
".........#....#.......#.#...##.",
"#....#......##.#.........##...#",
"#.............#..##.#.#..##....",
"...#....#..#...#....#.#.#.#...#",
".#....#....#..##.....#.#...###.",
"##............#.#...##.#..#.#..",
"##.#....##.....#..#..###....#..",
"##....#................##......",
"...##..#...#..###....#.....##..",
".#...##......#..#.#.....#...#..",
"..##......##...#.##.......#....",
"......#.....#.....##........#.#",
"##....#...........#............",
"#.......#....#..#.##..##.#..#..",
".#....##.#.....#..#..#.........",
".#....#.#.#...#.....##.....#.#.",
".......##.#.#........#......##.",
"##........#.##.......#...#..#..",
"...###..##....#.#....#.#.......",
"......#.......#...##.....#...#.",
"..#......##.#......#.....#.....",
".....#.....###...#.............",
"#...#.#...#...#..#......#......",
"#.....#.......###.#....###.#...",
"...#.......#....####....##..#..",
"#.#.....#....#........#.......#",
".........#.......#......#.#...#",
"..##....#.....##...............",
"..........#..#.#..#......#.....",
"..................##...##.#....",
"........#.......#...#..#.#.#...",
".....#.#..##..#..#.#..#.......#",
".....#........#..#..#....#....#",
"##............#..#..#...#....#.",
".....#....................##..#",
"........##.#....###............",
"##.......#.##................#.",
".....###.#..#..#...#....###.##.",
".#......#.#....#.....##.#......",
"...##......##.........#...#....",
"....####..............#........",
"#...#.#..##..##.........##.....",
"......#......#....#..#.........",
"#.....#.....#.##...............",
"..#.##..#...##.#.####..#....###",
"#..#......#....#.##..##...#.#..",
"#....#.......#.....#.....#.#...",
"##.......#.....##...#.....#....",
"...#...##..........#..##..##..#",
".###..#..##...#....#...#..#....",
"......##..###.......###...#....",
"....#...#.#.......#.##...##..##",
"#.#......#..##.#.#..#..#..#....",
"......#........#.......#.......",
"..........#.#.....##...........",
"......#..#........#..#.#..###..",
"##..#.............##..#........",
".........#....#.....#.........#",
".....#..##...#..#..##.##......#",
"###..#...........#.......#....#",
"...............#....#.#........",
".##.#...#.#........##....#.....",
".##.###...##..###....#...#...#.",
".##..#....#.#.#...#.#.#.#...#..",
".###.#...#.......#....#..#.....",
"..#..#.#.#.#........#.....##...",
".#.......#.#...#.#...........##",
"...#.....##....#.....##...#....",
"................#.....####...#.",
".#.#......#.......##...#.##....",
".###.........#.#......#..#.#...",
"#......#...#....#..##.......#..",
".##..#....#..#...........#...#.",
".#...#.......##........#.##....",
"..#...........#...##...........",
".....##....##......#....#..#...",
"#......#.#...#.##.#...##....#..",
"#....................#...##...#",
"..#............#........#......",
".............#.........##.....#",
"...#...#......##.#...#...#.#...",
"..#...#.#.................#....",
"....##...#....#...###.##......#",
"...#....#...#..#...#....#.....#",
"...##.#........#..#.........#..",
"..##.....#..##...#.....##...#..",
"#.........#.#.#...#......#...#.",
"#.#...........#...#..#..#..##..",
"..#..#..##....#..........#.###.",
".....#..#....#.#...#...#..#..#.",
"###.....#..#.................#.",
".#..##.##.#......#....##..#....",
};