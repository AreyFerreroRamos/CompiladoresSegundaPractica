function swap(v::Tensor{Int32})
    temp = v[1]
    v[1] = v[2]
    v[2] = temp
end

function mean(a::Float64, b::Float64)::Float64
    return (a + b) / 2
end


var = 2 +5
var2 = var + 8 * var - (var % 2)
var2x3x4 = [[[0, 1, 2, 3]; [4, 5, 6, 7]; [8, 9, 10, 11]]; [[12, 13, 14, 15]; [16, 17, 18, 19]; [20, 21, 22, 23]]]  #var2x3x4[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23]
var6 = var2x3x4[1,3,4]                          # var6 = var2x3x4[44]
var6 = var2x3x4[1,1,1]                          # var6 = var2x3x4[0]
var6 = var2x3x4[var,3,4]
var = var2x3x4[1,1,4] + var2x3x4[1,var6,3]      # var6 = var2x3x4[12] + (-)
var2x3x4[1,1,1] = 1                        # var2x3x4[0] = 1
var2x3x4[1,1,1] = var                           # var2x3x4[0] = 1
var2x3x4[1,var,1] = 1
var2x3x4[1,var,1] = var + var2x3x4[1,2,4] + var2x3x4[1,var6,4] # (-) = var + var2x3x4[28] = (-)
    