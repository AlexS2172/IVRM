/* @(#)omnifact.h	40.7 4/14/97 12:47:52 */

/*
    OMNIFACT.H - known OMnet facility types - V3.7
*/
#define OMNI_FACTYP_LOGIN ((uint32) 1)
#define OMNI_FACTYP_LOGOUT ((uint32) 2)
#define OMNI_FACTYP_LOCK ((uint32) 3)
#define OMNI_FACTYP_UNLOCK ((uint32) 4)
#define OMNI_FACTYP_SETSRC ((uint32) 5)
#define OMNI_FACTYP_MGMT ((uint32) 6)
#define OMNI_FACTYP_SET_PASSW ((uint32) 7)

#define OMNI_EVTTYP_DEDICATED 990	/* dedicated events */
#define OMNI_EVTTYP_ATTENTION 991	/* attention events */
#define OMNI_EVTTYP_ALL 999		/* read any event */
#define OMNI_EVTTYP_SHOW 1000		/* show all allowed event types */
#define OMNI_EVTTYP_NETWORK 1001	/* network status messages */
#define OMNI_EVTTYP_SHOW_SUBSCR 1002 	/* show all authorized Subscription info objects */

/*
    Predefined facility types 70 - 209

    (Exchange-defined facility types 210 - 329)
*/
#define OMNI_FACTYP_OM__XSEEP0 ((uint32) 70)
#define OMNI_FACTYP_OM__XSEEP1 ((uint32) 71)
#define OMNI_FACTYP_OM__XSEEP2 ((uint32) 72)
#define OMNI_FACTYP_OM__XSEEP3 ((uint32) 73)
#define OMNI_FACTYP_OM__XSEEP4 ((uint32) 74)
#define OMNI_FACTYP_OM__XSEEP5 ((uint32) 75)
#define OMNI_FACTYP_OM__XSEEP6 ((uint32) 76)
#define OMNI_FACTYP_OM__XSEEP7 ((uint32) 77)
#define OMNI_FACTYP_OM__XSEEP8 ((uint32) 78)
#define OMNI_FACTYP_OM__XSEEP9 ((uint32) 79)

#define OMNI_FACTYP_OM__XSEET0 ((uint32) 80)
#define OMNI_FACTYP_OM__XSEET1 ((uint32) 81)
#define OMNI_FACTYP_OM__XSEET2 ((uint32) 82)
#define OMNI_FACTYP_OM__XSEET3 ((uint32) 83)
#define OMNI_FACTYP_OM__XSEET4 ((uint32) 84)
#define OMNI_FACTYP_OM__XSEET5 ((uint32) 85)
#define OMNI_FACTYP_OM__XSEET6 ((uint32) 86)
#define OMNI_FACTYP_OM__XSEET7 ((uint32) 87)
#define OMNI_FACTYP_OM__XSEET8 ((uint32) 88)
#define OMNI_FACTYP_OM__XSEET9 ((uint32) 89)

#define OMNI_FACTYP_OML_XGBEP0 ((uint32) 90)
#define OMNI_FACTYP_OML_XGBEP1 ((uint32) 91)
#define OMNI_FACTYP_OML_XGBEP2 ((uint32) 92)
#define OMNI_FACTYP_OML_XGBEP3 ((uint32) 93)
#define OMNI_FACTYP_OML_XGBEP4 ((uint32) 94)
#define OMNI_FACTYP_OML_XGBEP5 ((uint32) 95)
#define OMNI_FACTYP_OML_XGBEP6 ((uint32) 96)
#define OMNI_FACTYP_OML_XGBEP7 ((uint32) 97)
#define OMNI_FACTYP_OML_XGBEP8 ((uint32) 98)
#define OMNI_FACTYP_OML_XGBEP9 ((uint32) 99)

#define OMNI_FACTYP_OML_XGBET0 ((uint32) 100)
#define OMNI_FACTYP_OML_XGBET1 ((uint32) 101)
#define OMNI_FACTYP_OML_XGBET2 ((uint32) 102)
#define OMNI_FACTYP_OML_XGBET3 ((uint32) 103)
#define OMNI_FACTYP_OML_XGBET4 ((uint32) 104)
#define OMNI_FACTYP_OML_XGBET5 ((uint32) 105)
#define OMNI_FACTYP_OML_XGBET6 ((uint32) 106)
#define OMNI_FACTYP_OML_XGBET7 ((uint32) 107)
#define OMNI_FACTYP_OML_XGBET8 ((uint32) 108)
#define OMNI_FACTYP_OML_XGBET9 ((uint32) 109)

#define OMNI_FACTYP_OTOBXATEP0 ((uint32) 110)
#define OMNI_FACTYP_OTOBXATEP1 ((uint32) 111)
#define OMNI_FACTYP_OTOBXATEP2 ((uint32) 112)
#define OMNI_FACTYP_OTOBXATEP3 ((uint32) 113)
#define OMNI_FACTYP_OTOBXATEP4 ((uint32) 114)
#define OMNI_FACTYP_OTOBXATEP5 ((uint32) 115)
#define OMNI_FACTYP_OTOBXATEP6 ((uint32) 116)
#define OMNI_FACTYP_OTOBXATEP7 ((uint32) 117)
#define OMNI_FACTYP_OTOBXATEP8 ((uint32) 118)
#define OMNI_FACTYP_OTOBXATEP9 ((uint32) 119)

#define OMNI_FACTYP_OTOBXATET0 ((uint32) 120)
#define OMNI_FACTYP_OTOBXATET1 ((uint32) 121)
#define OMNI_FACTYP_OTOBXATET2 ((uint32) 122)
#define OMNI_FACTYP_OTOBXATET3 ((uint32) 123)
#define OMNI_FACTYP_OTOBXATET4 ((uint32) 124)
#define OMNI_FACTYP_OTOBXATET5 ((uint32) 125)
#define OMNI_FACTYP_OTOBXATET6 ((uint32) 126)
#define OMNI_FACTYP_OTOBXATET7 ((uint32) 127)
#define OMNI_FACTYP_OTOBXATET8 ((uint32) 128)
#define OMNI_FACTYP_OTOBXATET9 ((uint32) 129)

#define OMNI_FACTYP_CED_XITEP0 ((uint32) 130)
#define OMNI_FACTYP_CED_XITEP1 ((uint32) 131)
#define OMNI_FACTYP_CED_XITEP2 ((uint32) 132)
#define OMNI_FACTYP_CED_XITEP3 ((uint32) 133)
#define OMNI_FACTYP_CED_XITEP4 ((uint32) 134)
#define OMNI_FACTYP_CED_XITEP5 ((uint32) 135)
#define OMNI_FACTYP_CED_XITEP6 ((uint32) 136)
#define OMNI_FACTYP_CED_XITEP7 ((uint32) 137)
#define OMNI_FACTYP_CED_XITEP8 ((uint32) 138)
#define OMNI_FACTYP_CED_XITEP9 ((uint32) 139)

#define OMNI_FACTYP_CED_XITET0 ((uint32) 140)
#define OMNI_FACTYP_CED_XITET1 ((uint32) 141)
#define OMNI_FACTYP_CED_XITET2 ((uint32) 142)
#define OMNI_FACTYP_CED_XITET3 ((uint32) 143)
#define OMNI_FACTYP_CED_XITET4 ((uint32) 144)
#define OMNI_FACTYP_CED_XITET5 ((uint32) 145)
#define OMNI_FACTYP_CED_XITET6 ((uint32) 146)
#define OMNI_FACTYP_CED_XITET7 ((uint32) 147)
#define OMNI_FACTYP_CED_XITET8 ((uint32) 148)
#define OMNI_FACTYP_CED_XITET9 ((uint32) 149)

#define OMNI_FACTYP_AMEXXUSEP0 ((uint32) 150)
#define OMNI_FACTYP_AMEXXUSEP1 ((uint32) 151)
#define OMNI_FACTYP_AMEXXUSEP2 ((uint32) 152)
#define OMNI_FACTYP_AMEXXUSEP3 ((uint32) 153)
#define OMNI_FACTYP_AMEXXUSEP4 ((uint32) 154)
#define OMNI_FACTYP_AMEXXUSEP5 ((uint32) 155)
#define OMNI_FACTYP_AMEXXUSEP6 ((uint32) 156)
#define OMNI_FACTYP_AMEXXUSEP7 ((uint32) 157)
#define OMNI_FACTYP_AMEXXUSEP8 ((uint32) 158)
#define OMNI_FACTYP_AMEXXUSEP9 ((uint32) 159)

#define OMNI_FACTYP_AMEXXUSET0 ((uint32) 160)
#define OMNI_FACTYP_AMEXXUSET1 ((uint32) 161)
#define OMNI_FACTYP_AMEXXUSET2 ((uint32) 162)
#define OMNI_FACTYP_AMEXXUSET3 ((uint32) 163)
#define OMNI_FACTYP_AMEXXUSET4 ((uint32) 164)
#define OMNI_FACTYP_AMEXXUSET5 ((uint32) 165)
#define OMNI_FACTYP_AMEXXUSET6 ((uint32) 166)
#define OMNI_FACTYP_AMEXXUSET7 ((uint32) 167)
#define OMNI_FACTYP_AMEXXUSET8 ((uint32) 168)
#define OMNI_FACTYP_AMEXXUSET9 ((uint32) 169)

#define OMNI_FACTYP_HKFEXHKEP0 ((uint32) 170)
#define OMNI_FACTYP_HKFEXHKEP1 ((uint32) 171)
#define OMNI_FACTYP_HKFEXHKEP2 ((uint32) 172)
#define OMNI_FACTYP_HKFEXHKEP3 ((uint32) 173)
#define OMNI_FACTYP_HKFEXHKEP4 ((uint32) 174)
#define OMNI_FACTYP_HKFEXHKEP5 ((uint32) 175)
#define OMNI_FACTYP_HKFEXHKEP6 ((uint32) 176)
#define OMNI_FACTYP_HKFEXHKEP7 ((uint32) 177)
#define OMNI_FACTYP_HKFEXHKEP8 ((uint32) 178)
#define OMNI_FACTYP_HKFEXHKEP9 ((uint32) 179)

#define OMNI_FACTYP_HKFEXHKET0 ((uint32) 180)
#define OMNI_FACTYP_HKFEXHKET1 ((uint32) 181)
#define OMNI_FACTYP_HKFEXHKET2 ((uint32) 182)
#define OMNI_FACTYP_HKFEXHKET3 ((uint32) 183)
#define OMNI_FACTYP_HKFEXHKET4 ((uint32) 184)
#define OMNI_FACTYP_HKFEXHKET5 ((uint32) 185)
#define OMNI_FACTYP_HKFEXHKET6 ((uint32) 186)
#define OMNI_FACTYP_HKFEXHKET7 ((uint32) 187)
#define OMNI_FACTYP_HKFEXHKET8 ((uint32) 188)
#define OMNI_FACTYP_HKFEXHKET9 ((uint32) 189)

#define OMNI_FACTYP_ASX_XAUEP0 ((uint32) 190)
#define OMNI_FACTYP_ASX_XAUEP1 ((uint32) 191)
#define OMNI_FACTYP_ASX_XAUEP2 ((uint32) 192)
#define OMNI_FACTYP_ASX_XAUEP3 ((uint32) 193)
#define OMNI_FACTYP_ASX_XAUEP4 ((uint32) 194)
#define OMNI_FACTYP_ASX_XAUEP5 ((uint32) 195)
#define OMNI_FACTYP_ASX_XAUEP6 ((uint32) 196)
#define OMNI_FACTYP_ASX_XAUEP7 ((uint32) 197)
#define OMNI_FACTYP_ASX_XAUEP8 ((uint32) 198)
#define OMNI_FACTYP_ASX_XAUEP9 ((uint32) 199)

#define OMNI_FACTYP_ASX_XAUET0 ((uint32) 200)
#define OMNI_FACTYP_ASX_XAUET1 ((uint32) 201)
#define OMNI_FACTYP_ASX_XAUET2 ((uint32) 202)
#define OMNI_FACTYP_ASX_XAUET3 ((uint32) 203)
#define OMNI_FACTYP_ASX_XAUET4 ((uint32) 204)
#define OMNI_FACTYP_ASX_XAUET5 ((uint32) 205)
#define OMNI_FACTYP_ASX_XAUET6 ((uint32) 206)
#define OMNI_FACTYP_ASX_XAUET7 ((uint32) 207)
#define OMNI_FACTYP_ASX_XAUET8 ((uint32) 208)
#define OMNI_FACTYP_ASX_XAUET9 ((uint32) 209)

#define OMNI_FACTYP_OB__XNOEP0 ((uint32) 210)
#define OMNI_FACTYP_OB__XNOEP1 ((uint32) 211)
#define OMNI_FACTYP_OB__XNOEP2 ((uint32) 212)
#define OMNI_FACTYP_OB__XNOEP3 ((uint32) 213)
#define OMNI_FACTYP_OB__XNOEP4 ((uint32) 214)
#define OMNI_FACTYP_OB__XNOEP5 ((uint32) 215)
#define OMNI_FACTYP_OB__XNOEP6 ((uint32) 216)
#define OMNI_FACTYP_OB__XNOEP7 ((uint32) 217)
#define OMNI_FACTYP_OB__XNOEP8 ((uint32) 218)
#define OMNI_FACTYP_OB__XNOEP9 ((uint32) 219)

#define OMNI_FACTYP_OB__XNOET0 ((uint32) 220)
#define OMNI_FACTYP_OB__XNOET1 ((uint32) 221)
#define OMNI_FACTYP_OB__XNOET2 ((uint32) 222)
#define OMNI_FACTYP_OB__XNOET3 ((uint32) 223)
#define OMNI_FACTYP_OB__XNOET4 ((uint32) 224)
#define OMNI_FACTYP_OB__XNOET5 ((uint32) 225)
#define OMNI_FACTYP_OB__XNOET6 ((uint32) 226)
#define OMNI_FACTYP_OB__XNOET7 ((uint32) 227)
#define OMNI_FACTYP_OB__XNOET8 ((uint32) 228)
#define OMNI_FACTYP_OB__XNOET9 ((uint32) 229)

#define OMNI_FACTYP_PX__XUSEP0 ((uint32) 230)
#define OMNI_FACTYP_PX__XUSEP1 ((uint32) 231)
#define OMNI_FACTYP_PX__XUSEP2 ((uint32) 232)
#define OMNI_FACTYP_PX__XUSEP3 ((uint32) 233)
#define OMNI_FACTYP_PX__XUSEP4 ((uint32) 234)
#define OMNI_FACTYP_PX__XUSEP5 ((uint32) 235)
#define OMNI_FACTYP_PX__XUSEP6 ((uint32) 236)
#define OMNI_FACTYP_PX__XUSEP7 ((uint32) 237)
#define OMNI_FACTYP_PX__XUSEP8 ((uint32) 238)
#define OMNI_FACTYP_PX__XUSEP9 ((uint32) 239)

#define OMNI_FACTYP_PX__XUSET0 ((uint32) 240)
#define OMNI_FACTYP_PX__XUSET1 ((uint32) 241)
#define OMNI_FACTYP_PX__XUSET2 ((uint32) 242)
#define OMNI_FACTYP_PX__XUSET3 ((uint32) 243)
#define OMNI_FACTYP_PX__XUSET4 ((uint32) 244)
#define OMNI_FACTYP_PX__XUSET5 ((uint32) 245)
#define OMNI_FACTYP_PX__XUSET6 ((uint32) 246)
#define OMNI_FACTYP_PX__XUSET7 ((uint32) 247)
#define OMNI_FACTYP_PX__XUSET8 ((uint32) 248)
#define OMNI_FACTYP_PX__XUSET9 ((uint32) 249)

#define OMNI_FACTYP_ASE_XGREP0 ((uint32) 250)
#define OMNI_FACTYP_ASE_XGREP1 ((uint32) 251)
#define OMNI_FACTYP_ASE_XGREP2 ((uint32) 252)
#define OMNI_FACTYP_ASE_XGREP3 ((uint32) 253)
#define OMNI_FACTYP_ASE_XGREP4 ((uint32) 254)
#define OMNI_FACTYP_ASE_XGREP5 ((uint32) 255)
#define OMNI_FACTYP_ASE_XGREP6 ((uint32) 256)
#define OMNI_FACTYP_ASE_XGREP7 ((uint32) 257)
#define OMNI_FACTYP_ASE_XGREP8 ((uint32) 258)
#define OMNI_FACTYP_ASE_XGREP9 ((uint32) 259)

#define OMNI_FACTYP_ASE_XGRET0 ((uint32) 260)
#define OMNI_FACTYP_ASE_XGRET1 ((uint32) 261)
#define OMNI_FACTYP_ASE_XGRET2 ((uint32) 262)
#define OMNI_FACTYP_ASE_XGRET3 ((uint32) 263)
#define OMNI_FACTYP_ASE_XGRET4 ((uint32) 264)
#define OMNI_FACTYP_ASE_XGRET5 ((uint32) 265)
#define OMNI_FACTYP_ASE_XGRET6 ((uint32) 266)
#define OMNI_FACTYP_ASE_XGRET7 ((uint32) 267)
#define OMNI_FACTYP_ASE_XGRET8 ((uint32) 268)
#define OMNI_FACTYP_ASE_XGRET9 ((uint32) 269)

#define OMNI_FACTYP_ISE_XUSEP0 ((uint32) 270)
#define OMNI_FACTYP_ISE_XUSEP1 ((uint32) 271)
#define OMNI_FACTYP_ISE_XUSEP2 ((uint32) 272)
#define OMNI_FACTYP_ISE_XUSEP3 ((uint32) 273)
#define OMNI_FACTYP_ISE_XUSEP4 ((uint32) 274)
#define OMNI_FACTYP_ISE_XUSEP5 ((uint32) 275)
#define OMNI_FACTYP_ISE_XUSEP6 ((uint32) 276)
#define OMNI_FACTYP_ISE_XUSEP7 ((uint32) 277)
#define OMNI_FACTYP_ISE_XUSEP8 ((uint32) 278)
#define OMNI_FACTYP_ISE_XUSEP9 ((uint32) 279)

#define OMNI_FACTYP_ISE_XUSET0 ((uint32) 280)
#define OMNI_FACTYP_ISE_XUSET1 ((uint32) 281)
#define OMNI_FACTYP_ISE_XUSET2 ((uint32) 282)
#define OMNI_FACTYP_ISE_XUSET3 ((uint32) 283)
#define OMNI_FACTYP_ISE_XUSET4 ((uint32) 284)
#define OMNI_FACTYP_ISE_XUSET5 ((uint32) 285)
#define OMNI_FACTYP_ISE_XUSET6 ((uint32) 286)
#define OMNI_FACTYP_ISE_XUSET7 ((uint32) 287)
#define OMNI_FACTYP_ISE_XUSET8 ((uint32) 288)
#define OMNI_FACTYP_ISE_XUSET9 ((uint32) 289)

/* Toronto Stock exchange TSE */
#define OMNI_FACTYP_TSE_XCAEP0 ((uint32) 290)
#define OMNI_FACTYP_TSE_XCAEP1 ((uint32) 291)
#define OMNI_FACTYP_TSE_XCAEP2 ((uint32) 292)
#define OMNI_FACTYP_TSE_XCAEP3 ((uint32) 293)
#define OMNI_FACTYP_TSE_XCAEP4 ((uint32) 294)
#define OMNI_FACTYP_TSE_XCAEP5 ((uint32) 295)
#define OMNI_FACTYP_TSE_XCAEP6 ((uint32) 296)
#define OMNI_FACTYP_TSE_XCAEP7 ((uint32) 297)
#define OMNI_FACTYP_TSE_XCAEP8 ((uint32) 298)
#define OMNI_FACTYP_TSE_XCAEP9 ((uint32) 299)

#define OMNI_FACTYP_TSE_XCAET0 ((uint32) 300)
#define OMNI_FACTYP_TSE_XCAET1 ((uint32) 301)
#define OMNI_FACTYP_TSE_XCAET2 ((uint32) 302)
#define OMNI_FACTYP_TSE_XCAET3 ((uint32) 303)
#define OMNI_FACTYP_TSE_XCAET4 ((uint32) 304)
#define OMNI_FACTYP_TSE_XCAET5 ((uint32) 305)
#define OMNI_FACTYP_TSE_XCAET6 ((uint32) 306)
#define OMNI_FACTYP_TSE_XCAET7 ((uint32) 307)
#define OMNI_FACTYP_TSE_XCAET8 ((uint32) 308)
#define OMNI_FACTYP_TSE_XCAET9 ((uint32) 309)

/* Korea Futures Exchange KOFEX */
#define OMNI_FACTYP_KFE_XKREP0 ((uint32) 310)
#define OMNI_FACTYP_KFE_XKREP1 ((uint32) 311)
#define OMNI_FACTYP_KFE_XKREP2 ((uint32) 312)
#define OMNI_FACTYP_KFE_XKREP3 ((uint32) 313)
#define OMNI_FACTYP_KFE_XKREP4 ((uint32) 314)
#define OMNI_FACTYP_KFE_XKREP5 ((uint32) 315)
#define OMNI_FACTYP_KFE_XKREP6 ((uint32) 316)
#define OMNI_FACTYP_KFE_XKREP7 ((uint32) 317)
#define OMNI_FACTYP_KFE_XKREP8 ((uint32) 318)
#define OMNI_FACTYP_KFE_XKREP9 ((uint32) 319)

#define OMNI_FACTYP_KFE_XKRET0 ((uint32) 320)
#define OMNI_FACTYP_KFE_XKRET1 ((uint32) 321)
#define OMNI_FACTYP_KFE_XKRET2 ((uint32) 322)
#define OMNI_FACTYP_KFE_XKRET3 ((uint32) 323)
#define OMNI_FACTYP_KFE_XKRET4 ((uint32) 324)
#define OMNI_FACTYP_KFE_XKRET5 ((uint32) 325)
#define OMNI_FACTYP_KFE_XKRET6 ((uint32) 326)
#define OMNI_FACTYP_KFE_XKRET7 ((uint32) 327)
#define OMNI_FACTYP_KFE_XKRET8 ((uint32) 328)
#define OMNI_FACTYP_KFE_XKRET9 ((uint32) 329)

/* OMES */
#define OMNI_FACTYP_OMESXSEEP0 ((uint32) 330)
#define OMNI_FACTYP_OMESXSEEP1 ((uint32) 331)
#define OMNI_FACTYP_OMESXSEEP2 ((uint32) 332)
#define OMNI_FACTYP_OMESXSEEP3 ((uint32) 333)
#define OMNI_FACTYP_OMESXSEEP4 ((uint32) 334)
#define OMNI_FACTYP_OMESXSEEP5 ((uint32) 335)
#define OMNI_FACTYP_OMESXSEEP6 ((uint32) 336)
#define OMNI_FACTYP_OMESXSEEP7 ((uint32) 337)
#define OMNI_FACTYP_OMESXSEEP8 ((uint32) 338)
#define OMNI_FACTYP_OMESXSEEP9 ((uint32) 339)

#define OMNI_FACTYP_OMESXSEET0 ((uint32) 340)
#define OMNI_FACTYP_OMESXSEET1 ((uint32) 341)
#define OMNI_FACTYP_OMESXSEET2 ((uint32) 342)
#define OMNI_FACTYP_OMESXSEET3 ((uint32) 343)
#define OMNI_FACTYP_OMESXSEET4 ((uint32) 344)
#define OMNI_FACTYP_OMESXSEET5 ((uint32) 345)
#define OMNI_FACTYP_OMESXSEET6 ((uint32) 346)
#define OMNI_FACTYP_OMESXSEET7 ((uint32) 347)
#define OMNI_FACTYP_OMESXSEET8 ((uint32) 348)
#define OMNI_FACTYP_OMESXSEET9 ((uint32) 349)

/* CalPX */
#define OMNI_FACTYP_PXF_XUSEP0 ((uint32) 350)
#define OMNI_FACTYP_PXF_XUSEP1 ((uint32) 351)
#define OMNI_FACTYP_PXF_XUSEP2 ((uint32) 352)
#define OMNI_FACTYP_PXF_XUSEP3 ((uint32) 353)
#define OMNI_FACTYP_PXF_XUSEP4 ((uint32) 354)
#define OMNI_FACTYP_PXF_XUSEP5 ((uint32) 355)
#define OMNI_FACTYP_PXF_XUSEP6 ((uint32) 356)
#define OMNI_FACTYP_PXF_XUSEP7 ((uint32) 357)
#define OMNI_FACTYP_PXF_XUSEP8 ((uint32) 358)
#define OMNI_FACTYP_PXF_XUSEP9 ((uint32) 359)

#define OMNI_FACTYP_PXF_XUSET0 ((uint32) 360)
#define OMNI_FACTYP_PXF_XUSET1 ((uint32) 361)
#define OMNI_FACTYP_PXF_XUSET2 ((uint32) 362)
#define OMNI_FACTYP_PXF_XUSET3 ((uint32) 363)
#define OMNI_FACTYP_PXF_XUSET4 ((uint32) 364)
#define OMNI_FACTYP_PXF_XUSET5 ((uint32) 365)
#define OMNI_FACTYP_PXF_XUSET6 ((uint32) 366)
#define OMNI_FACTYP_PXF_XUSET7 ((uint32) 367)
#define OMNI_FACTYP_PXF_XUSET8 ((uint32) 368)
#define OMNI_FACTYP_PXF_XUSET9 ((uint32) 369)

/* BrokerTec US */
#define OMNI_FACTYP_BTUSXUSEP0 ((uint32) 370)
#define OMNI_FACTYP_BTUSXUSEP1 ((uint32) 371)
#define OMNI_FACTYP_BTUSXUSEP2 ((uint32) 372)
#define OMNI_FACTYP_BTUSXUSEP3 ((uint32) 373)
#define OMNI_FACTYP_BTUSXUSEP4 ((uint32) 374)
#define OMNI_FACTYP_BTUSXUSEP5 ((uint32) 375)
#define OMNI_FACTYP_BTUSXUSEP6 ((uint32) 376)
#define OMNI_FACTYP_BTUSXUSEP7 ((uint32) 377)
#define OMNI_FACTYP_BTUSXUSEP8 ((uint32) 378)
#define OMNI_FACTYP_BTUSXUSEP9 ((uint32) 379)

#define OMNI_FACTYP_BTUSXUSET0 ((uint32) 380)
#define OMNI_FACTYP_BTUSXUSET1 ((uint32) 381)
#define OMNI_FACTYP_BTUSXUSET2 ((uint32) 382)
#define OMNI_FACTYP_BTUSXUSET3 ((uint32) 383)
#define OMNI_FACTYP_BTUSXUSET4 ((uint32) 384)
#define OMNI_FACTYP_BTUSXUSET5 ((uint32) 385)
#define OMNI_FACTYP_BTUSXUSET6 ((uint32) 386)
#define OMNI_FACTYP_BTUSXUSET7 ((uint32) 387)
#define OMNI_FACTYP_BTUSXUSET8 ((uint32) 388)
#define OMNI_FACTYP_BTUSXUSET9 ((uint32) 389)

/* BrokerTec EU */
#define OMNI_FACTYP_BTEUXGBEP0 ((uint32) 390)
#define OMNI_FACTYP_BTEUXGBEP1 ((uint32) 391)
#define OMNI_FACTYP_BTEUXGBEP2 ((uint32) 392)
#define OMNI_FACTYP_BTEUXGBEP3 ((uint32) 393)
#define OMNI_FACTYP_BTEUXGBEP4 ((uint32) 394)
#define OMNI_FACTYP_BTEUXGBEP5 ((uint32) 395)
#define OMNI_FACTYP_BTEUXGBEP6 ((uint32) 396)
#define OMNI_FACTYP_BTEUXGBEP7 ((uint32) 397)
#define OMNI_FACTYP_BTEUXGBEP8 ((uint32) 398)
#define OMNI_FACTYP_BTEUXGBEP9 ((uint32) 399)

#define OMNI_FACTYP_BTEUXGBET0 ((uint32) 400)
#define OMNI_FACTYP_BTEUXGBET1 ((uint32) 401)
#define OMNI_FACTYP_BTEUXGBET2 ((uint32) 402)
#define OMNI_FACTYP_BTEUXGBET3 ((uint32) 403)
#define OMNI_FACTYP_BTEUXGBET4 ((uint32) 404)
#define OMNI_FACTYP_BTEUXGBET5 ((uint32) 405)
#define OMNI_FACTYP_BTEUXGBET6 ((uint32) 406)
#define OMNI_FACTYP_BTEUXGBET7 ((uint32) 407)
#define OMNI_FACTYP_BTEUXGBET8 ((uint32) 408)
#define OMNI_FACTYP_BTEUXGBET9 ((uint32) 409)

/* Power UK */
#define OMNI_FACTYP_UKPXXGBEP0 ((uint32) 410)
#define OMNI_FACTYP_UKPXXGBEP1 ((uint32) 411)
#define OMNI_FACTYP_UKPXXGBEP2 ((uint32) 412)
#define OMNI_FACTYP_UKPXXGBEP3 ((uint32) 413)
#define OMNI_FACTYP_UKPXXGBEP4 ((uint32) 414)
#define OMNI_FACTYP_UKPXXGBEP5 ((uint32) 415)
#define OMNI_FACTYP_UKPXXGBEP6 ((uint32) 416)
#define OMNI_FACTYP_UKPXXGBEP7 ((uint32) 417)
#define OMNI_FACTYP_UKPXXGBEP8 ((uint32) 418)
#define OMNI_FACTYP_UKPXXGBEP9 ((uint32) 419)

#define OMNI_FACTYP_UKPXXGBET0 ((uint32) 420)
#define OMNI_FACTYP_UKPXXGBET1 ((uint32) 421)
#define OMNI_FACTYP_UKPXXGBET2 ((uint32) 422)
#define OMNI_FACTYP_UKPXXGBET3 ((uint32) 423)
#define OMNI_FACTYP_UKPXXGBET4 ((uint32) 424)
#define OMNI_FACTYP_UKPXXGBET5 ((uint32) 425)
#define OMNI_FACTYP_UKPXXGBET6 ((uint32) 426)
#define OMNI_FACTYP_UKPXXGBET7 ((uint32) 427)
#define OMNI_FACTYP_UKPXXGBET8 ((uint32) 428)
#define OMNI_FACTYP_UKPXXGBET9 ((uint32) 429)

/* JWX_ */
#define OMNI_FACTYP_JWX_XGBEP0 ((uint32) 430)
#define OMNI_FACTYP_JWX_XGBEP1 ((uint32) 431)
#define OMNI_FACTYP_JWX_XGBEP2 ((uint32) 432)
#define OMNI_FACTYP_JWX_XGBEP3 ((uint32) 433)
#define OMNI_FACTYP_JWX_XGBEP4 ((uint32) 434)
#define OMNI_FACTYP_JWX_XGBEP5 ((uint32) 435)
#define OMNI_FACTYP_JWX_XGBEP6 ((uint32) 436)
#define OMNI_FACTYP_JWX_XGBEP7 ((uint32) 437)
#define OMNI_FACTYP_JWX_XGBEP8 ((uint32) 438)
#define OMNI_FACTYP_JWX_XGBEP9 ((uint32) 439)

#define OMNI_FACTYP_JWX_XGBET0 ((uint32) 440)
#define OMNI_FACTYP_JWX_XGBET1 ((uint32) 441)
#define OMNI_FACTYP_JWX_XGBET2 ((uint32) 442)
#define OMNI_FACTYP_JWX_XGBET3 ((uint32) 443)
#define OMNI_FACTYP_JWX_XGBET4 ((uint32) 444)
#define OMNI_FACTYP_JWX_XGBET5 ((uint32) 445)
#define OMNI_FACTYP_JWX_XGBET6 ((uint32) 446)
#define OMNI_FACTYP_JWX_XGBET7 ((uint32) 447)
#define OMNI_FACTYP_JWX_XGBET8 ((uint32) 448)
#define OMNI_FACTYP_JWX_XGBET9 ((uint32) 449)

/* BOTC */
#define OMNI_FACTYP_BOTCXUSEP0 ((uint32) 450)
#define OMNI_FACTYP_BOTCXUSEP1 ((uint32) 451)
#define OMNI_FACTYP_BOTCXUSEP2 ((uint32) 452)
#define OMNI_FACTYP_BOTCXUSEP3 ((uint32) 453)
#define OMNI_FACTYP_BOTCXUSEP4 ((uint32) 454)
#define OMNI_FACTYP_BOTCXUSEP5 ((uint32) 455)
#define OMNI_FACTYP_BOTCXUSEP6 ((uint32) 456)
#define OMNI_FACTYP_BOTCXUSEP7 ((uint32) 457)
#define OMNI_FACTYP_BOTCXUSEP8 ((uint32) 458)
#define OMNI_FACTYP_BOTCXUSEP9 ((uint32) 459)

#define OMNI_FACTYP_BOTCXUSET0 ((uint32) 460)
#define OMNI_FACTYP_BOTCXUSET1 ((uint32) 461)
#define OMNI_FACTYP_BOTCXUSET2 ((uint32) 462)
#define OMNI_FACTYP_BOTCXUSET3 ((uint32) 463)
#define OMNI_FACTYP_BOTCXUSET4 ((uint32) 464)
#define OMNI_FACTYP_BOTCXUSET5 ((uint32) 465)
#define OMNI_FACTYP_BOTCXUSET6 ((uint32) 466)
#define OMNI_FACTYP_BOTCXUSET7 ((uint32) 467)
#define OMNI_FACTYP_BOTCXUSET8 ((uint32) 468)
#define OMNI_FACTYP_BOTCXUSET9 ((uint32) 469)

/* LPX_ */
#define OMNI_FACTYP_LPX_XDEEP0 ((uint32) 470)
#define OMNI_FACTYP_LPX_XDEEP1 ((uint32) 471)
#define OMNI_FACTYP_LPX_XDEEP2 ((uint32) 472)
#define OMNI_FACTYP_LPX_XDEEP3 ((uint32) 473)
#define OMNI_FACTYP_LPX_XDEEP4 ((uint32) 474)
#define OMNI_FACTYP_LPX_XDEEP5 ((uint32) 475)
#define OMNI_FACTYP_LPX_XDEEP6 ((uint32) 476)
#define OMNI_FACTYP_LPX_XDEEP7 ((uint32) 477)
#define OMNI_FACTYP_LPX_XDEEP8 ((uint32) 478)
#define OMNI_FACTYP_LPX_XDEEP9 ((uint32) 479)

#define OMNI_FACTYP_LPX_XDEET0 ((uint32) 480)
#define OMNI_FACTYP_LPX_XDEET1 ((uint32) 481)
#define OMNI_FACTYP_LPX_XDEET2 ((uint32) 482)
#define OMNI_FACTYP_LPX_XDEET3 ((uint32) 483)
#define OMNI_FACTYP_LPX_XDEET4 ((uint32) 484)
#define OMNI_FACTYP_LPX_XDEET5 ((uint32) 485)
#define OMNI_FACTYP_LPX_XDEET6 ((uint32) 486)
#define OMNI_FACTYP_LPX_XDEET7 ((uint32) 487)
#define OMNI_FACTYP_LPX_XDEET8 ((uint32) 488)
#define OMNI_FACTYP_LPX_XDEET9 ((uint32) 489)

/* SFE_ */
#define OMNI_FACTYP_SFE_XAUEP0 ((uint32) 490)
#define OMNI_FACTYP_SFE_XAUEP1 ((uint32) 491)
#define OMNI_FACTYP_SFE_XAUEP2 ((uint32) 492)
#define OMNI_FACTYP_SFE_XAUEP3 ((uint32) 493)
#define OMNI_FACTYP_SFE_XAUEP4 ((uint32) 494)
#define OMNI_FACTYP_SFE_XAUEP5 ((uint32) 495)
#define OMNI_FACTYP_SFE_XAUEP6 ((uint32) 496)
#define OMNI_FACTYP_SFE_XAUEP7 ((uint32) 497)
#define OMNI_FACTYP_SFE_XAUEP8 ((uint32) 498)
#define OMNI_FACTYP_SFE_XAUEP9 ((uint32) 499)

#define OMNI_FACTYP_SFE_XAUET0 ((uint32) 500)
#define OMNI_FACTYP_SFE_XAUET1 ((uint32) 501)
#define OMNI_FACTYP_SFE_XAUET2 ((uint32) 502)
#define OMNI_FACTYP_SFE_XAUET3 ((uint32) 503)
#define OMNI_FACTYP_SFE_XAUET4 ((uint32) 504)
#define OMNI_FACTYP_SFE_XAUET5 ((uint32) 505)
#define OMNI_FACTYP_SFE_XAUET6 ((uint32) 506)
#define OMNI_FACTYP_SFE_XAUET7 ((uint32) 507)
#define OMNI_FACTYP_SFE_XAUET8 ((uint32) 508)
#define OMNI_FACTYP_SFE_XAUET9 ((uint32) 509)

/* LME_ */
#define OMNI_FACTYP_LME_XGBEP0 ((uint32) 510)
#define OMNI_FACTYP_LME_XGBEP1 ((uint32) 511)
#define OMNI_FACTYP_LME_XGBEP2 ((uint32) 512)
#define OMNI_FACTYP_LME_XGBEP3 ((uint32) 513)
#define OMNI_FACTYP_LME_XGBEP4 ((uint32) 514)
#define OMNI_FACTYP_LME_XGBEP5 ((uint32) 515)
#define OMNI_FACTYP_LME_XGBEP6 ((uint32) 516)
#define OMNI_FACTYP_LME_XGBEP7 ((uint32) 517)
#define OMNI_FACTYP_LME_XGBEP8 ((uint32) 518)
#define OMNI_FACTYP_LME_XGBEP9 ((uint32) 519)

#define OMNI_FACTYP_LME_XGBET0 ((uint32) 520)
#define OMNI_FACTYP_LME_XGBET1 ((uint32) 521)
#define OMNI_FACTYP_LME_XGBET2 ((uint32) 522)
#define OMNI_FACTYP_LME_XGBET3 ((uint32) 523)
#define OMNI_FACTYP_LME_XGBET4 ((uint32) 524)
#define OMNI_FACTYP_LME_XGBET5 ((uint32) 525)
#define OMNI_FACTYP_LME_XGBET6 ((uint32) 526)
#define OMNI_FACTYP_LME_XGBET7 ((uint32) 527)
#define OMNI_FACTYP_LME_XGBET8 ((uint32) 528)
#define OMNI_FACTYP_LME_XGBET9 ((uint32) 529)

/* AMEQ */
#define OMNI_FACTYP_AMEQXUSEP0 ((uint32) 530)
#define OMNI_FACTYP_AMEQXUSEP1 ((uint32) 531)
#define OMNI_FACTYP_AMEQXUSEP2 ((uint32) 532)
#define OMNI_FACTYP_AMEQXUSEP3 ((uint32) 533)
#define OMNI_FACTYP_AMEQXUSEP4 ((uint32) 534)
#define OMNI_FACTYP_AMEQXUSEP5 ((uint32) 535)
#define OMNI_FACTYP_AMEQXUSEP6 ((uint32) 536)
#define OMNI_FACTYP_AMEQXUSEP7 ((uint32) 537)
#define OMNI_FACTYP_AMEQXUSEP8 ((uint32) 538)
#define OMNI_FACTYP_AMEQXUSEP9 ((uint32) 539)

#define OMNI_FACTYP_AMEQXUSET0 ((uint32) 540)
#define OMNI_FACTYP_AMEQXUSET1 ((uint32) 541)
#define OMNI_FACTYP_AMEQXUSET2 ((uint32) 542)
#define OMNI_FACTYP_AMEQXUSET3 ((uint32) 543)
#define OMNI_FACTYP_AMEQXUSET4 ((uint32) 544)
#define OMNI_FACTYP_AMEQXUSET5 ((uint32) 545)
#define OMNI_FACTYP_AMEQXUSET6 ((uint32) 546)
#define OMNI_FACTYP_AMEQXUSET7 ((uint32) 547)
#define OMNI_FACTYP_AMEQXUSET8 ((uint32) 548)
#define OMNI_FACTYP_AMEQXUSET9 ((uint32) 549)
