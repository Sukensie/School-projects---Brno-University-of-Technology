-- uart_fsm.vhd: UART controller - finite state machine
-- Author(s): Tomáš Souček (xsouce15)
--
library ieee;
use ieee.std_logic_1164.all;

-------------------------------------------------
entity UART_FSM is
port(
   CLK : in std_logic;
   RST : in std_logic;
   DIN : in std_logic;
   CNT : in std_logic_vector(4 downto 0); --potřebuju maximálně hodnotu 24... proto pole 5 bitů
   CNT_BIT : in std_logic_vector(3 downto 0); --stačí pole 3 bitů (maxílní hodnota 8)
   GET_DATA_TOGGLE : out std_logic;
   CNT_TOGGLE : out std_logic;
   DOUT_VLD : out std_logic
   );
end entity UART_FSM;

-------------------------------------------------
architecture behavioral of UART_FSM is
type STATE_TYPE is (START_BIT, FIRST_BIT, GET_DATA, STOP_BIT, DATA_VALID);
signal state : STATE_TYPE := START_BIT;

begin
   DOUT_VLD <= '1' when state = DATA_VALID else '0';
   GET_DATA_TOGGLE <= '1' when state = GET_DATA else '0';
   CNT_TOGGLE <= '1' when state = FIRST_BIT or state = GET_DATA or state = STOP_BIT else '0';

  

   process(CLK) begin
         if rising_edge(CLK) then
            if RST = '1' then
               state <= START_BIT;
            else
               case( state ) is
                  when START_BIT => if DIN = '0' then state <= FIRST_BIT; end if;

                  when FIRST_BIT => if CNT = "01000" then state <= GET_DATA; end if;-- spustit po 8 bitech

                  when GET_DATA => if CNT_BIT = "1000" then state <= STOP_BIT;  end if; --čeká na přečtení všech 8 bitů

                  when STOP_BIT => if CNT = "01000" then state <= DATA_VALID; end if;-- spustit po 8 bitech

                  when DATA_VALID => state <= START_BIT;
                                    
                  when others =>   null;          
               end case;
            end if; 
         end if;
   end process;
end behavioral;
