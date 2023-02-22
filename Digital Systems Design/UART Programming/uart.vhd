-- uart.vhd: UART controller - receiving part
-- Author(s): Tomáš Souček (xsouce15)
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

-------------------------------------------------
entity UART_RX is
port(	
    CLK: 	    in std_logic;
	RST: 	    in std_logic;
	DIN: 	    in std_logic;
	DOUT: 	    out std_logic_vector(7 downto 0);
	DOUT_VLD: 	out std_logic
);
end UART_RX;  

-------------------------------------------------
architecture behavioral of UART_RX is
	signal cnt : std_logic_vector(4 downto 0);
	signal cnt_bit : std_logic_vector(3 downto 0);
	signal data_valid : std_logic;
	signal get_data_toggle: std_logic; 
	signal cnt_toggle : std_logic; 

begin
	FSM: entity work.UART_FSM(behavioral)
	port map (
		CLK => CLK,
		RST => RST,
		DIN => DIN,
		CNT => CNT,
		CNT_BIT => cnt_bit,
		GET_DATA_TOGGLE => GET_DATA_TOGGLE,
		CNT_TOGGLE => CNT_TOGGLE,
		DOUT_VLD => data_valid
	);

	process(CLK) begin
		if RST = '1' then
			DOUT <= "00000000";
		end if;

		if rising_edge(CLK) then
			if CNT_TOGGLE = '1' then
				cnt <= cnt + 1;
			else
				cnt <= "00000";
			end if ;

		
			if get_data_toggle = '1' then
				if cnt(4) = '1' then
					cnt <= "00000";
					case( cnt_bit ) is
						when "0000" => DOUT(0) <= DIN;
						when "0001" => DOUT(1) <= DIN;
						when "0010" => DOUT(2) <= DIN;
						when "0011" => DOUT(3) <= DIN;
						when "0100" => DOUT(4) <= DIN;
						when "0101" => DOUT(5) <= DIN;
						when "0110" => DOUT(6) <= DIN;
						when "0111" => DOUT(7) <= DIN;  
						when others => null;					
					end case;
					cnt_bit <= cnt_bit + 1;
				end if;
			else
				cnt_bit <= "0000";
			end if;
		
			if data_valid = '1' then
				DOUT_VLD <= '1';
			else
				DOUT_VLD <= '0';
			end if;

		end if;
	end process;
end behavioral;
