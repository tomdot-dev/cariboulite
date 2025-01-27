`include "spi_if.v"
`include "sys_ctrl.v"
`include "io_ctrl.v"
`include "smi_ctrl.v"

module top(
      input i_glob_clock,
      output i_smi_a0_irq,
      input i_smi_a1_alt_clock,

      // RF FRONT-END PATH
      output o_rx_h_tx_l,
      output o_rx_h_tx_l_b,
      output o_tr_vc1,
      output o_tr_vc1_b,
      output o_tr_vc2,
      output o_shdn_rx_lna,
      output o_shdn_tx_lna,

      // MODEM (LVDS & CLOCK)
      input i_radio_clk,
      output o_iq_tx_p,
      output o_iq_tx_n,
      output o_iq_tx_clk_p,
      output o_iq_tx_clk_n,
      input i_iq_rx_09_p,     // Paired with i_iq_rx_09_n - only the 'B' pins need to be specified
      input i_iq_rx_24_n,     // Paired with i_iq_rx_24_p - only the 'B' pins need to be specified
      input i_iq_rx_clk_p,    // Paired with i_iq_rx_clk_n - only the 'B' pins need to be specified

      // Note: The icestorm (specifically nextpnr) fails to build if both diff pins are constrained
      //       in the constrain file and the interface herein. Thus we need to take the out so that
      //       it will "understand" we actually want an LVDS pair inputs. In addition, the pair is
      //       defined only by the "B" pins in BANK3 and not the "A" pins (which is counter-logical)

      // MIXER
      output o_mixer_fm,
      output o_mixer_en,

      // DIGITAL I/F
      input [3:0] i_config,
      input i_button,
      output o_ldo_2v8_en,
      output [7:0] io_pmod,
      output o_led0,
      output o_led1,

      // SMI TO RPI
      input i_smi_a2,
      input i_smi_a3,

      input i_smi_soe_se,
      input i_smi_swe_srw,
      inout [7:0] io_smi_data,

      // SPI
      input i_mosi,
      input i_sck,
      input i_ss,
      output o_miso );

   //=========================================================================
   // INNER SIGNALS
   //=========================================================================
   reg [1:0]   r_counter;
   wire        w_clock_spi;
   wire        w_clock_sys;
   wire [4:0]  w_ioc;
   wire [7:0]  w_rx_data;
   reg [7:0]   r_tx_data;
   wire [3:0]  w_cs;
   wire        w_fetch;
   wire        w_load;
   reg         r_reset_b;

   wire [7:0]  w_tx_data_sys;
   wire [7:0]  w_tx_data_io;
   wire [7:0]  w_tx_data_smi;

   //=========================================================================
   // INITIAL STATE
   //=========================================================================
   initial begin
      r_counter = 2'b00;
      r_reset_b = 1'b1;
   end

   //=========================================================================
   // INSTANCES
   //=========================================================================
   spi_if spi_if_ins
   (
      .i_rst_b (r_reset_b),
      .i_sys_clk (w_clock_spi),
      .o_ioc (w_ioc),
      .o_data_in (w_rx_data),
      .i_data_out (r_tx_data),
      .o_cs (w_cs),
      .o_fetch_cmd (w_fetch),
      .o_load_cmd (w_load),

      // SPI Interface
      .i_spi_sck (i_sck),
      .o_spi_miso (int_miso),
      .i_spi_mosi (i_mosi),
      .i_spi_cs_b (i_ss)
   );

   wire int_miso;
   assign o_miso = (i_ss)?1'bZ:int_miso;

   sys_ctrl sys_ctrl_ins
   (
      .i_rst_b (r_reset),
      .i_sys_clk (w_clock_sys),
      .i_ioc (w_ioc),
      .i_data_in (w_rx_data),
      .o_data_out (w_tx_data_sys),
      .i_cs (w_cs[0]),
      .i_fetch_cmd (w_fetch),
      .i_load_cmd (w_load)
   );

   io_ctrl io_ctrl_ins
   (
      .i_rst_b (r_reset),
      .i_sys_clk (w_clock_sys),
      .i_ioc (w_ioc),
      .i_data_in (w_rx_data),
      .o_data_out (w_tx_data_io),
      .i_cs (w_cs[1]),
      .i_fetch_cmd (w_fetch),
      .i_load_cmd (w_load),

      /// Digital interfaces
      .i_button (i_button),
      .i_config (i_config),
      .o_ldo_2v8_en (o_ldo_2v8_en),
      .o_led0 (o_led0),
      .o_led1 (o_led1),
      .o_pmod (),

      // Analog interfaces
      .o_mixer_fm (o_mixer_fm),
      .o_rx_h_tx_l (o_rx_h_tx_l),
      .o_rx_h_tx_l_b (o_rx_h_tx_l_b),
      .o_tr_vc1 (o_tr_vc1),
      .o_tr_vc1_b (o_tr_vc1_b),
      .o_tr_vc2 (o_tr_vc2),
      .o_shdn_tx_lna (o_shdn_tx_lna),
      .o_shdn_rx_lna (o_shdn_rx_lna),
      .o_mixer_en (o_mixer_en)
   );

   smi_ctrl smi_ctrl_ins
   (
      .i_rst_b (r_reset),
      .i_sys_clk (w_clock_sys),
      .i_ioc (w_ioc),
      .i_data_in (w_rx_data),
      .o_data_out (w_tx_data_smi),
      .i_cs (w_cs[2]),
      .i_fetch_cmd (w_fetch),
      .i_load_cmd (w_load)
   );

   //=========================================================================
   // CONBINATORIAL ASSIGNMENTS
   //=========================================================================
   assign w_clock_spi = r_counter[0];
   assign w_clock_sys = r_counter[0];

   //=========================================================================
   // CLOCK AND DATA-FLOW
   //=========================================================================
   always @(posedge i_glob_clock)
   begin
      r_counter <= r_counter + 1;

      case (w_cs)
         4'b0001: r_tx_data <= w_tx_data_sys;
         4'b0010: r_tx_data <= w_tx_data_io;
         4'b0100: r_tx_data <= w_tx_data_smi;
         4'b1000: r_tx_data <= 8'b10100101;  // reserved
         4'b0000: r_tx_data <= 8'b00000000;  // no module selected
      endcase
   end

   //=========================================================================
   // I/O (SB_IO, SB_GB) DIFFERENTIAL LINES
   //=========================================================================
   // Differential clock signal
   wire lvds_clock;        // The direct clock input
   wire lvds_clock_buf;    // The clock input after global buffer (improved fanout)

   SB_IO #(
      .PIN_TYPE(6'b000001),         // Input only, direct mode
      .IO_STANDARD("SB_LVDS_INPUT") // LVDS input
   ) iq_rx_clk (
      .PACKAGE_PIN(i_iq_rx_clk_p),  // Physical connection to 'i_iq_rx_clk_p'
      .D_IN_0 ( lvds_clock ));      // Wire out to 'lvds_clock'

   SB_GB lvds_clk_buffer (          // Improve 'lvds_clock' fanout by pushing it into
                                    // a global high-fanout buffer
      .USER_SIGNAL_TO_GLOBAL_BUFFER (lvds_clock),
      .GLOBAL_BUFFER_OUTPUT(lvds_clock_buf) );

   // Testing - output the clock signal (positive and negative) to the PMOD
   assign io_pmod[0] = lvds_clock_buf;
   assign io_pmod[1] = ~lvds_clock_buf;

   // Differential 2.4GHz I/Q DDR signal
   SB_IO #(
      .PIN_TYPE(6'b000000),         // Input only, DDR mode (sample on both pos edge and
                                    // negedge of the input clock)
      .IO_STANDARD("SB_LVDS_INPUT"),// LVDS standard
      .NEG_TRIGGER(1'b0)            // We may need to specify it as 1'b1 to negate the signal
   ) iq_rx_24 (
      .PACKAGE_PIN(i_iq_rx_24_n),   // Attention: this is the 'n' input, thus the actual values
                                    //            will need to be negated (PCB layout constraint)
      .INPUT_CLK (lvds_clock_buf),  // The I/O sampling clock with DDR
      .D_IN_0 ( io_pmod[2] ),       // the 0 deg data output
      .D_IN_1 ( io_pmod[3]) );      // the 180 deg data output

   // Differential 0.9GHz I/Q DDR signal
   SB_IO #(
      .PIN_TYPE(6'b000000),         // Input only, DDR mode (sample on both pos edge and
                                    // negedge of the input clock)
      .IO_STANDARD("SB_LVDS_INPUT") // LVDS standard
   ) iq_rx_09 (
      .PACKAGE_PIN(i_iq_rx_09_p),
      .INPUT_CLK (lvds_clock_buf),  // The I/O sampling clock with DDR
      .D_IN_0 ( io_pmod[4] ),       // the 0 deg data output
      .D_IN_1 ( io_pmod[5]) );      // the 180 deg data output

endmodule // top
