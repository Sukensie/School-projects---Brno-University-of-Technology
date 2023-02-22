///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1995/2010 Xilinx, Inc.
// All Right Reserved.
///////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor : Xilinx
// \   \   \/     Version : 10.1
//  \   \         Description : Xilinx TEST ONLY Library Component
//  /   /                  Delay Element.
// /___/   /\     Filename : ZHOLD_DELAY.v
// \   \  /  \    Timestamp : Wed Apr 14 15:49:29 PDT 2010
//  \___\/\___\
//
// Revision:
//    04/14/10 - Initial version.
// End Revision

`timescale  1 ps / 1 ps

module ZHOLD_DELAY (
          DLYFABRIC,
          DLYIFF,
          DLYIN
       );

       parameter integer IDELAY_VALUE     = 0;
       parameter integer IFF_DELAY_VALUE = 0;
       parameter ZHOLD_FABRIC = "FALSE";
       parameter ZHOLD_IFF = "FALSE";

       output DLYFABRIC;
       output DLYIFF;

       input  DLYIN;



       tri0  GSR = glbl.GSR;

//------------------- constants ------------------------------------
       localparam MAX_IFF_DELAY_COUNT = 31;
       localparam MIN_IFF_DELAY_COUNT = 0;

       localparam MAX_IDELAY_COUNT = 31;
       localparam MIN_IDELAY_COUNT = 0;

       

       real TAP_DELAY = 200.0;

       integer idelay_count=0;
       integer iff_idelay_count=0;

       // inputs
       wire dlyin_in;
       wire gsr_in; 

       // outputs
       reg tap_out_fabric = 0;
       reg tap_out_iff = 0;
 

//----------------------------------------------------------------------
//-------------------------------  Output ------------------------------
//----------------------------------------------------------------------
       assign DLYFABRIC = tap_out_fabric;
       assign DLYIFF = tap_out_iff;

//----------------------------------------------------------------------
//-------------------------------  Input -------------------------------
//----------------------------------------------------------------------
       assign dlyin_in = DLYIN;



//------------------------------------------------------------
//---------------------   Initialization  --------------------
//------------------------------------------------------------
       initial begin

        //-------- ZHOLD_FABRIC check
           case (ZHOLD_FABRIC)
               "TRUE"  : idelay_count = IDELAY_VALUE;
               "FALSE" : idelay_count = 0;
               default : begin
                  $display("Attribute Syntax Error : The attribute ZHOLD_FABRIC on ZHOLD_DELAY instance %m is set to %s.  Legal values for this attribute are TRUE or FALSE.",  ZHOLD_FABRIC);
                  $finish;
               end
           endcase

        //-------- ZHOLD_IFF check
           case (ZHOLD_IFF)
               "TRUE"  : iff_idelay_count = IFF_DELAY_VALUE;
               "FALSE" : iff_idelay_count = 0;
               default : begin
                  $display("Attribute Syntax Error : The attribute ZHOLD_IFF on ZHOLD_DELAY instance %m is set to %s.  Legal values for this attribute are TRUE or FALSE.",  ZHOLD_IFF);
                  $finish;
               end
           endcase

        //-------- IDELAY_VALUE check
           if (IDELAY_VALUE < MIN_IDELAY_COUNT || IDELAY_VALUE > MAX_IDELAY_COUNT) begin
               $display("Attribute Syntax Error : The attribute IDELAY_VALUE on ZHOLD_DELAY instance %m is set to %d.  Legal values for this attribute are 0, 1, 2, 3, .... or 31", IDELAY_VALUE);
               $finish;

           end


        //-------- IFF_DELAY_VALUE check
           if (IFF_DELAY_VALUE < MIN_IFF_DELAY_COUNT || IFF_DELAY_VALUE > MAX_IFF_DELAY_COUNT) begin
               $display("Attribute Syntax Error : The attribute IFF_DELAY_VALUE on ZHOLD_DELAY instance %m is set to %d.  Legal values for this attribute are 0, 1, 2, 3, .... or 31", IFF_DELAY_VALUE);
               $finish;

           end

       end // initial begin


//*********************************************************
//*** DELAY IDATA signal
//*********************************************************
       assign                delay_chain_0  = dlyin_in;
       assign #TAP_DELAY delay_chain_1  = delay_chain_0;
       assign #TAP_DELAY delay_chain_2  = delay_chain_1;
       assign #TAP_DELAY delay_chain_3  = delay_chain_2;
       assign #TAP_DELAY delay_chain_4  = delay_chain_3;
       assign #TAP_DELAY delay_chain_5  = delay_chain_4;
       assign #TAP_DELAY delay_chain_6  = delay_chain_5;
       assign #TAP_DELAY delay_chain_7  = delay_chain_6;
       assign #TAP_DELAY delay_chain_8  = delay_chain_7;
       assign #TAP_DELAY delay_chain_9  = delay_chain_8;
       assign #TAP_DELAY delay_chain_10 = delay_chain_9;
       assign #TAP_DELAY delay_chain_11 = delay_chain_10;
       assign #TAP_DELAY delay_chain_12 = delay_chain_11;
       assign #TAP_DELAY delay_chain_13 = delay_chain_12;
       assign #TAP_DELAY delay_chain_14 = delay_chain_13;
       assign #TAP_DELAY delay_chain_15 = delay_chain_14;
       assign #TAP_DELAY delay_chain_16 = delay_chain_15;
       assign #TAP_DELAY delay_chain_17 = delay_chain_16;
       assign #TAP_DELAY delay_chain_18 = delay_chain_17;
       assign #TAP_DELAY delay_chain_19 = delay_chain_18;
       assign #TAP_DELAY delay_chain_20 = delay_chain_19;
       assign #TAP_DELAY delay_chain_21 = delay_chain_20;
       assign #TAP_DELAY delay_chain_22 = delay_chain_21;
       assign #TAP_DELAY delay_chain_23 = delay_chain_22;
       assign #TAP_DELAY delay_chain_24 = delay_chain_23;
       assign #TAP_DELAY delay_chain_25 = delay_chain_24;
       assign #TAP_DELAY delay_chain_26 = delay_chain_25;
       assign #TAP_DELAY delay_chain_27 = delay_chain_26;
       assign #TAP_DELAY delay_chain_28 = delay_chain_27;
       assign #TAP_DELAY delay_chain_29 = delay_chain_28;
       assign #TAP_DELAY delay_chain_30 = delay_chain_29;
       assign #TAP_DELAY delay_chain_31 = delay_chain_30;

//*********************************************************
//*** assign delay
//*********************************************************
       always @(idelay_count) begin
           case (idelay_count)
               0:  assign tap_out_fabric = delay_chain_0;
               1:  assign tap_out_fabric = delay_chain_1;
               2:  assign tap_out_fabric = delay_chain_2;
               3:  assign tap_out_fabric = delay_chain_3;
               4:  assign tap_out_fabric = delay_chain_4;
               5:  assign tap_out_fabric = delay_chain_5;
               6:  assign tap_out_fabric = delay_chain_6;
               7:  assign tap_out_fabric = delay_chain_7;
               8:  assign tap_out_fabric = delay_chain_8;
               9:  assign tap_out_fabric = delay_chain_9;
               10: assign tap_out_fabric = delay_chain_10;
               11: assign tap_out_fabric = delay_chain_11;
               12: assign tap_out_fabric = delay_chain_12;
               13: assign tap_out_fabric = delay_chain_13;
               14: assign tap_out_fabric = delay_chain_14;
               15: assign tap_out_fabric = delay_chain_15;
               16: assign tap_out_fabric = delay_chain_16;
               17: assign tap_out_fabric = delay_chain_17;
               18: assign tap_out_fabric = delay_chain_18;
               19: assign tap_out_fabric = delay_chain_19;
               20: assign tap_out_fabric = delay_chain_20;
               21: assign tap_out_fabric = delay_chain_21;
               22: assign tap_out_fabric = delay_chain_22;
               23: assign tap_out_fabric = delay_chain_23;
               24: assign tap_out_fabric = delay_chain_24;
               25: assign tap_out_fabric = delay_chain_25;
               26: assign tap_out_fabric = delay_chain_26;
               27: assign tap_out_fabric = delay_chain_27;
               28: assign tap_out_fabric = delay_chain_28;
               29: assign tap_out_fabric = delay_chain_29;
               30: assign tap_out_fabric = delay_chain_30;
               31: assign tap_out_fabric = delay_chain_31;
               default:
                   assign tap_out_fabric = delay_chain_0;
           endcase
       end // always @ (idelay_count)

       always @(iff_idelay_count) begin
           case (iff_idelay_count)
               0:  assign tap_out_iff = delay_chain_0;
               1:  assign tap_out_iff = delay_chain_1;
               2:  assign tap_out_iff = delay_chain_2;
               3:  assign tap_out_iff = delay_chain_3;
               4:  assign tap_out_iff = delay_chain_4;
               5:  assign tap_out_iff = delay_chain_5;
               6:  assign tap_out_iff = delay_chain_6;
               7:  assign tap_out_iff = delay_chain_7;
               8:  assign tap_out_iff = delay_chain_8;
               9:  assign tap_out_iff = delay_chain_9;
               10: assign tap_out_iff = delay_chain_10;
               11: assign tap_out_iff = delay_chain_11;
               12: assign tap_out_iff = delay_chain_12;
               13: assign tap_out_iff = delay_chain_13;
               14: assign tap_out_iff = delay_chain_14;
               15: assign tap_out_iff = delay_chain_15;
               16: assign tap_out_iff = delay_chain_16;
               17: assign tap_out_iff = delay_chain_17;
               18: assign tap_out_iff = delay_chain_18;
               19: assign tap_out_iff = delay_chain_19;
               20: assign tap_out_iff = delay_chain_20;
               21: assign tap_out_iff = delay_chain_21;
               22: assign tap_out_iff = delay_chain_22;
               23: assign tap_out_iff = delay_chain_23;
               24: assign tap_out_iff = delay_chain_24;
               25: assign tap_out_iff = delay_chain_25;
               26: assign tap_out_iff = delay_chain_26;
               27: assign tap_out_iff = delay_chain_27;
               28: assign tap_out_iff = delay_chain_28;
               29: assign tap_out_iff = delay_chain_29;
               30: assign tap_out_iff = delay_chain_30;
               31: assign tap_out_iff = delay_chain_31;
               default:
                   assign tap_out_iff = delay_chain_0;
           endcase
       end // always @ (iff_idelay_count)


    specify
       specparam PATHPULSE$ = 0;

    endspecify

endmodule // X_XHOLD_DELAY

