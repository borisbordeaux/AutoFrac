state("s", 1)
subdivision("s0s", "s", "s")
------------------------------------------------------------
-- all edges states
state("B4", 1)
boundary("b0", "B4", "s")
boundary("b1", "B4", "s")
permutation("permut", "B4", "B4")
------------------------------------------------------------
-- all edges impl
subdivision("s0", "B4", "B4")
subdivision("s1", "B4", "B4")
subdivision("s2", "B4", "B4")
subdivision("s3", "B4", "B4")
space("B4", { "b0", "intern_0", "b1" })
constraint("B4", { "permut", "b0" }, { "b1" })
constraint("B4", { "permut", "intern_0" }, { "intern_0" })
constraint("B4", { "permut", "b1" }, { "b0" })
constraint("B4", { "permut", "s0" }, { "s3", "permut" })
constraint("B4", { "permut", "s1" }, { "s2", "permut" })
constraint("B4", { "permut", "s2" }, { "s1", "permut" })
constraint("B4", { "permut", "s3" }, { "s0", "permut" })
initMat("B4", "s0", {
    { 1.0000, 0.7500, 0.5625 },
    { 0.0000, 0.2500, 0.3750 },
    { 0.0000, 0.0000, 0.0625 }
}, "CONST")
initMat("B4", "s1", {
    { 0.5625, 0.3750, 0.2500 },
    { 0.3750, 0.5000, 0.5000 },
    { 0.0625, 0.1250, 0.2500 }
}, "CONST")
initMat("B4", "s2", {
    { 0.2500, 0.1250, 0.0625 },
    { 0.5000, 0.5000, 0.3750 },
    { 0.2500, 0.3750, 0.5625 }
}, "CONST")
initMat("B4", "s3", {
    { 0.0625, 0.0000, 0.0000 },
    { 0.3750, 0.2500, 0.0000 },
    { 0.5625, 0.7500, 1.0000 }
}, "CONST")
------------------------------------------------------------
-- all cells states
state("Cell_0", 0)
------------------------------------------------------------
-- subd of init
subdivision("s0", "init", "Cell_0")
------------------------------------------------------------
-- edges of all states
boundary("b0", "Cell_0", "B4")
boundary("b1", "Cell_0", "B4")
boundary("b2", "Cell_0", "B4")
------------------------------------------------------------
-- subdivisions of all states
for i=0,18,1 do
    subdivision("s" .. i, "Cell_0", "Cell_0")
end
------------------------------------------------------------
-- spaces of all states
-- subdivisions of all states
space("Cell_0", { "b0", "b1", "b2" })
------------------------------------------------------------
-- grid of all states
grid("Cell_0", {
    { { "b0", "b0" }, { "b0", "intern_0" }, { "b0", "b1" } },
    { { "b1", "b0" }, { "b1", "intern_0" }, { "b1", "b1" } },
    { { "b2", "b0" }, { "b2", "intern_0" }, { "b2", "b1" } }
})
------------------------------------------------------------
-- prim of all states
primitive("Cell_0", {
    {
        { "b0", "s0", "b0" },
        { "b0", "s1", "b0" },
        { "b0", "s2", "b0" },
        { "b0", "s3", "b0" },
        { "b1", "s0", "b0" },
        { "b1", "s1", "b0" },
        { "b1", "s2", "b0" },
        { "b1", "s3", "b0" },
        { "b2", "s0", "b0" },
        { "b2", "s1", "b0" },
        { "b2", "s2", "b0" },
        { "b2", "s3", "b0" }
    }
})
------------------------------------------------------------
-- constraints of all states
-- incidence constraints
constraint("Cell_0", { "b0", "s2", "permut" }, { "s1", "b2" })
constraint("Cell_0", { "b0", "s2", "permut" }, { "s1", "b2" })
constraint("Cell_0", { "b0", "s3", "permut" }, { "s11", "b1" })
constraint("Cell_0", { "b0", "s0", "permut" }, { "s15", "b0" })
constraint("Cell_0", { "b0", "s1", "permut" }, { "s16", "b2" })
constraint("Cell_0", { "b1", "s2", "permut" }, { "s0", "b1" })
constraint("Cell_0", { "b1", "s3", "permut" }, { "s7", "b1" })
constraint("Cell_0", { "b1", "s0", "permut" }, { "s11", "b0" })
constraint("Cell_0", { "b1", "s1", "permut" }, { "s12", "b2" })
constraint("Cell_0", { "b2", "s2", "permut" }, { "s3", "b0" })
constraint("Cell_0", { "b2", "s0", "permut" }, { "s7", "b0" })
constraint("Cell_0", { "b2", "s1", "permut" }, { "s8", "b2" })
constraint("Cell_0", { "b2", "s3", "permut" }, { "s15", "b1" })
-- adjacency constraints
constraint("Cell_0", { "s0", "b0", "b1" }, { "s7", "b1", "b1" } )
constraint("Cell_0", { "s0", "b1", "b1" }, { "s12", "b1", "b1" } )
constraint("Cell_0", { "s0", "b2", "b1" }, { "s17", "b1", "b1" } )
constraint("Cell_0", { "s1", "b0", "b1" }, { "s9", "b1", "b1" } )
constraint("Cell_0", { "s1", "b1", "b1" }, { "s11", "b1", "b1" } )
constraint("Cell_0", { "s1", "b2", "b1" }, { "s16", "b1", "b1" } )
constraint("Cell_0", { "s2", "b0", "b1" }, { "s12", "b0", "b1" } )
constraint("Cell_0", { "s2", "b1", "b1" }, { "s9", "b0", "b1" } )
constraint("Cell_0", { "s2", "b2", "b1" }, { "s18", "b0", "b1" } )
constraint("Cell_0", { "s3", "b0", "b1" }, { "s8", "b1", "b1" } )
constraint("Cell_0", { "s3", "b1", "b1" }, { "s13", "b1", "b1" } )
constraint("Cell_0", { "s3", "b2", "b1" }, { "s15", "b1", "b1" } )
constraint("Cell_0", { "s4", "b0", "b1" }, { "s14", "b0", "b1" } )
constraint("Cell_0", { "s4", "b1", "b1" }, { "s8", "b0", "b1" } )
constraint("Cell_0", { "s4", "b2", "b1" }, { "s17", "b0", "b1" } )
constraint("Cell_0", { "s5", "b0", "b1" }, { "s13", "b0", "b1" } )
constraint("Cell_0", { "s5", "b1", "b1" }, { "s10", "b0", "b1" } )
constraint("Cell_0", { "s5", "b2", "b1" }, { "s16", "b0", "b1" } )
constraint("Cell_0", { "s6", "b0", "b1" }, { "s10", "b1", "b1" } )
constraint("Cell_0", { "s6", "b1", "b1" }, { "s14", "b1", "b1" } )
constraint("Cell_0", { "s6", "b2", "b1" }, { "s18", "b1", "b1" } )
constraint("Cell_0", { "s7", "b2", "b1" }, { "s8", "b2", "b1" } )
constraint("Cell_0", { "s9", "b2", "b1" }, { "s10", "b2", "b1" } )
constraint("Cell_0", { "s11", "b2", "b1" }, { "s12", "b2", "b1" } )
constraint("Cell_0", { "s13", "b2", "b1" }, { "s14", "b2", "b1" } )
constraint("Cell_0", { "s15", "b2", "b1" }, { "s16", "b2", "b1" } )
constraint("Cell_0", { "s17", "b2", "b1" }, { "s18", "b2", "b1" } )
-- edges adjacency constraints
constraint("Cell_0", { "b0", "b1" }, { "b1", "b0" })
constraint("Cell_0", { "b1", "b1" }, { "b2", "b0" })
constraint("Cell_0", { "b2", "b1" }, { "b0", "b0" })