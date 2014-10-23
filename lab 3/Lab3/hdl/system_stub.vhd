-------------------------------------------------------------------------------
-- system_stub.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity system_stub is
  port (
    fpga_0_RS232_Uart_1_RX_pin : in std_logic;
    fpga_0_RS232_Uart_1_TX_pin : out std_logic;
    sys_clk_pin : in std_logic;
    sys_rst_pin : in std_logic;
    LEDs_ext : inout std_logic_vector(0 to 3);
    INPUTS_ext : inout std_logic_vector(0 to 8)
  );
end system_stub;

architecture STRUCTURE of system_stub is

  component system is
    port (
      fpga_0_RS232_Uart_1_RX_pin : in std_logic;
      fpga_0_RS232_Uart_1_TX_pin : out std_logic;
      sys_clk_pin : in std_logic;
      sys_rst_pin : in std_logic;
      LEDs_ext : inout std_logic_vector(0 to 3);
      INPUTS_ext : inout std_logic_vector(0 to 8)
    );
  end component;

begin

  system_i : system
    port map (
      fpga_0_RS232_Uart_1_RX_pin => fpga_0_RS232_Uart_1_RX_pin,
      fpga_0_RS232_Uart_1_TX_pin => fpga_0_RS232_Uart_1_TX_pin,
      sys_clk_pin => sys_clk_pin,
      sys_rst_pin => sys_rst_pin,
      LEDs_ext => LEDs_ext,
      INPUTS_ext => INPUTS_ext
    );

end architecture STRUCTURE;

