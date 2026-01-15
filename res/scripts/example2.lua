state("s", 1)
subdivision("s0s", "s", "s")
------------------------------------------------------------
-- all edges states
state("B4", 1)
boundary("b0B4", "B4", "s")
boundary("b1B4", "B4", "s")
permutation("permutB4", "B4", "B4")
------------------------------------------------------------
-- all edges impl
subdivision("s0B4", "B4", "B4")
subdivision("s1B4", "B4", "B4")
subdivision("s2B4", "B4", "B4")
subdivision("s3B4", "B4", "B4")
space("B4", { "b0B4", "B4_intern_0", "b1B4" })
constraint({ "permutB4", "b0B4" }, { "b1B4" })
constraint({ "permutB4", "B4_intern_0" }, { "B4_intern_0" })
constraint({ "permutB4", "b1B4" }, { "b0B4" })
constraint({ "permutB4", "s0B4" }, { "s3B4", "permutB4" })
constraint({ "permutB4", "s1B4" }, { "s2B4", "permutB4" })
constraint({ "permutB4", "s2B4" }, { "s1B4", "permutB4" })
constraint({ "permutB4", "s3B4" }, { "s0B4", "permutB4" })
initMat("s0B4", {
    { 1.0000, 0.7500, 0.5625 },
    { 0.0000, 0.2500, 0.3750 },
    { 0.0000, 0.0000, 0.0625 }
}, "CONST")
initMat("s1B4", {
    { 0.5625, 0.3750, 0.2500 },
    { 0.3750, 0.5000, 0.5000 },
    { 0.0625, 0.1250, 0.2500 }
}, "CONST")
initMat("s2B4", {
    { 0.2500, 0.1250, 0.0625 },
    { 0.5000, 0.5000, 0.3750 },
    { 0.2500, 0.3750, 0.5625 }
}, "CONST")
initMat("s3B4", {
    { 0.0625, 0.0000, 0.0000 },
    { 0.3750, 0.2500, 0.0000 },
    { 0.5625, 0.7500, 1.0000 }
}, "CONST")
------------------------------------------------------------
-- all cells states
state("Cell_0", 0)
------------------------------------------------------------
-- subd of init
subdivision("s0init", "init", "Cell_0")
------------------------------------------------------------
-- edges of all states
boundary("b0Cell_0", "Cell_0", "B4")
boundary("b1Cell_0", "Cell_0", "B4")
boundary("b2Cell_0", "Cell_0", "B4")
------------------------------------------------------------
-- subdivisions of all states
for i=0,18,1 do
    subdivision("s" .. i .. "Cell_0", "Cell_0", "Cell_0")
end
------------------------------------------------------------
-- spaces of all states
-- subdivisions of all states
space("Cell_0", { "b0Cell_0", "b1Cell_0", "b2Cell_0" })
------------------------------------------------------------
-- grid of all states
grid("Cell_0", {
    { { "b0Cell_0", "b0B4" }, { "b0Cell_0", "B4_intern_0" }, { "b0Cell_0", "b1B4" } },
    { { "b1Cell_0", "b0B4" }, { "b1Cell_0", "B4_intern_0" }, { "b1Cell_0", "b1B4" } },
    { { "b2Cell_0", "b0B4" }, { "b2Cell_0", "B4_intern_0" }, { "b2Cell_0", "b1B4" } }
})
------------------------------------------------------------
-- prim of all states
primitive("Cell_0", {
    {
        { "b0Cell_0", "s0B4", "b0B4" },
        { "b0Cell_0", "s1B4", "b0B4" },
        { "b0Cell_0", "s2B4", "b0B4" },
        { "b0Cell_0", "s3B4", "b0B4" },
        { "b1Cell_0", "s0B4", "b0B4" },
        { "b1Cell_0", "s1B4", "b0B4" },
        { "b1Cell_0", "s2B4", "b0B4" },
        { "b1Cell_0", "s3B4", "b0B4" },
        { "b2Cell_0", "s0B4", "b0B4" },
        { "b2Cell_0", "s1B4", "b0B4" },
        { "b2Cell_0", "s2B4", "b0B4" },
        { "b2Cell_0", "s3B4", "b0B4" }
    }
})
------------------------------------------------------------
-- constraints of all states
-- incidence constraints
constraint({ "b0Cell_0", "s2B4", "permutB4" }, { "s1Cell_0", "b2Cell_0" })
constraint({ "b0Cell_0", "s2B4", "permutB4" }, { "s1Cell_0", "b2Cell_0" })
constraint({ "b0Cell_0", "s3B4", "permutB4" }, { "s11Cell_0", "b1Cell_0" })
constraint({ "b0Cell_0", "s0B4", "permutB4" }, { "s15Cell_0", "b0Cell_0" })
constraint({ "b0Cell_0", "s1B4", "permutB4" }, { "s16Cell_0", "b2Cell_0" })
constraint({ "b1Cell_0", "s2B4", "permutB4" }, { "s0Cell_0", "b1Cell_0" })
constraint({ "b1Cell_0", "s3B4", "permutB4" }, { "s7Cell_0", "b1Cell_0" })
constraint({ "b1Cell_0", "s0B4", "permutB4" }, { "s11Cell_0", "b0Cell_0" })
constraint({ "b1Cell_0", "s1B4", "permutB4" }, { "s12Cell_0", "b2Cell_0" })
constraint({ "b2Cell_0", "s2B4", "permutB4" }, { "s3Cell_0", "b0Cell_0" })
constraint({ "b2Cell_0", "s0B4", "permutB4" }, { "s7Cell_0", "b0Cell_0" })
constraint({ "b2Cell_0", "s1B4", "permutB4" }, { "s8Cell_0", "b2Cell_0" })
constraint({ "b2Cell_0", "s3B4", "permutB4" }, { "s15Cell_0", "b1Cell_0" })
-- adjacency constraints
constraint({ "s0Cell_0", "b0Cell_0", "b1B4" }, { "s7Cell_0", "b1Cell_0", "b1B4" } )
constraint({ "s0Cell_0", "b1Cell_0", "b1B4" }, { "s12Cell_0", "b1Cell_0", "b1B4" } )
constraint({ "s0Cell_0", "b2Cell_0", "b1B4" }, { "s17Cell_0", "b1Cell_0", "b1B4" } )
constraint({ "s1Cell_0", "b0Cell_0", "b1B4" }, { "s9Cell_0", "b1Cell_0", "b1B4" } )
constraint({ "s1Cell_0", "b1Cell_0", "b1B4" }, { "s11Cell_0", "b1Cell_0", "b1B4" } )
constraint({ "s1Cell_0", "b2Cell_0", "b1B4" }, { "s16Cell_0", "b1Cell_0", "b1B4" } )
constraint({ "s2Cell_0", "b0Cell_0", "b1B4" }, { "s12Cell_0", "b0Cell_0", "b1B4" } )
constraint({ "s2Cell_0", "b1Cell_0", "b1B4" }, { "s9Cell_0", "b0Cell_0", "b1B4" } )
constraint({ "s2Cell_0", "b2Cell_0", "b1B4" }, { "s18Cell_0", "b0Cell_0", "b1B4" } )
constraint({ "s3Cell_0", "b0Cell_0", "b1B4" }, { "s8Cell_0", "b1Cell_0", "b1B4" } )
constraint({ "s3Cell_0", "b1Cell_0", "b1B4" }, { "s13Cell_0", "b1Cell_0", "b1B4" } )
constraint({ "s3Cell_0", "b2Cell_0", "b1B4" }, { "s15Cell_0", "b1Cell_0", "b1B4" } )
constraint({ "s4Cell_0", "b0Cell_0", "b1B4" }, { "s14Cell_0", "b0Cell_0", "b1B4" } )
constraint({ "s4Cell_0", "b1Cell_0", "b1B4" }, { "s8Cell_0", "b0Cell_0", "b1B4" } )
constraint({ "s4Cell_0", "b2Cell_0", "b1B4" }, { "s17Cell_0", "b0Cell_0", "b1B4" } )
constraint({ "s5Cell_0", "b0Cell_0", "b1B4" }, { "s13Cell_0", "b0Cell_0", "b1B4" } )
constraint({ "s5Cell_0", "b1Cell_0", "b1B4" }, { "s10Cell_0", "b0Cell_0", "b1B4" } )
constraint({ "s5Cell_0", "b2Cell_0", "b1B4" }, { "s16Cell_0", "b0Cell_0", "b1B4" } )
constraint({ "s6Cell_0", "b0Cell_0", "b1B4" }, { "s10Cell_0", "b1Cell_0", "b1B4" } )
constraint({ "s6Cell_0", "b1Cell_0", "b1B4" }, { "s14Cell_0", "b1Cell_0", "b1B4" } )
constraint({ "s6Cell_0", "b2Cell_0", "b1B4" }, { "s18Cell_0", "b1Cell_0", "b1B4" } )
constraint({ "s7Cell_0", "b2Cell_0", "b1B4" }, { "s8Cell_0", "b2Cell_0", "b1B4" } )
constraint({ "s9Cell_0", "b2Cell_0", "b1B4" }, { "s10Cell_0", "b2Cell_0", "b1B4" } )
constraint({ "s11Cell_0", "b2Cell_0", "b1B4" }, { "s12Cell_0", "b2Cell_0", "b1B4" } )
constraint({ "s13Cell_0", "b2Cell_0", "b1B4" }, { "s14Cell_0", "b2Cell_0", "b1B4" } )
constraint({ "s15Cell_0", "b2Cell_0", "b1B4" }, { "s16Cell_0", "b2Cell_0", "b1B4" } )
constraint({ "s17Cell_0", "b2Cell_0", "b1B4" }, { "s18Cell_0", "b2Cell_0", "b1B4" } )
-- edges adjacency constraints
constraint({ "b0Cell_0", "b1B4" }, { "b1Cell_0", "b0B4" })
constraint({ "b1Cell_0", "b1B4" }, { "b2Cell_0", "b0B4" })
constraint({ "b2Cell_0", "b1B4" }, { "b0Cell_0", "b0B4" })