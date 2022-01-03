(3*5+(2.0-1))/2                                                                                                         # 8
var1 = div(8,4)                                                                                                         # var1 = 2
var2 = ""                                                                                                               # var2 = ""   
var3 = var2 * "Hola " * "mundo"                                                                                         # var3 = "Hola mundo"
var4 = 5 *5 + (8+2)/4 + 6.5                                                                                             # 33.5
var5 = ( true && false) || 5>2                                                                                          #true (1)
var5 = ( true && false) || 5<2                                                                                          #false (0)
var2x3x4 = [[[0, 1, 2, 3]; [4, 5, 6, 7]; [8, 9, 10, 11]]; [[12, 13, 14, 15]; [16, 17, 18, 19]; [20, 21, 22, 23]]]  	#var2x3x4[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23]
var6 =3                                                                                                                 #var6 = 3
var6 = var2x3x4[0,2,var6]                                                                                               #var6 = 11
var6 = var2x3x4[1,0,0]                                                                                                  #var6 = 12
var6 = var2x3x4[1,2,3]                                                                                                  #var6 = 23
var2x3x4[0,0,0] = var6                                                                                                  #var2x3x4 at pos 0 = 23
var2x3x4[0,0,1] = 102                                                                                                   #var2x3x4 at pos 0 = 102
var2x3x4[0,0,2] = 103                                                                                                   #var2x3x4 at pos 0 = 103
var2x3x4[0,0,3] = 104                                                                                                   #var2x3x4 at pos 0 = 104
var = var2x3x4[0,0,3] + var2x3x4[0,0,3]                                                                                 #var = 208
var2x3x4                                                                                                                #var2x3x4[23,102,103,104,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,]
varfinal = (var2x3x4 +var2x3x4) +( var2x3x4 +var2x3x4 -var2x3x4)                                                        #varfinal[69,306,309,312,12,15,18,21,24,27,30,33,36,39,42,45,48,51,54,57,60,63,66,69,]
multMatrix = 4 * var2x3x4 *4                                                                          #multMatrix[368,1632,1648,1664,64,80,96,112,128,144,160,176,192,208,224,240,256,272,288,304,320,336,352,368,]
vector = [1,2]                                                                                                          #vector[1,2,]
matrix4x2= [[0,1];[2,3];[4,5];[6,7]]                                                                                    #matrix4x2[0,1,2,3,4,5,6,7,]
resulta = matrix4x2*vector                                                                                              #resulta[2,8,14,20,]
vector = [0,1,2,3]                                                                                                      #vector[0,1,2,3,]
resulta = vector* matrix4x2                                                                                             #resulta[28,34,]
matrix2x4= [[0,1,2,3];[4,5,6,7]]                                                                                        #matrix2x4[0,1,2,3,4,5,6,7,]         
resulta = matrix2x4 *matrix4x2                                                                                          #resulta[28,34,76,98,]
resulta = matrix4x2 * matrix2x4 * matrix4x2                                                                             #resulta[76,98,284,362,492,626,700,890,]
