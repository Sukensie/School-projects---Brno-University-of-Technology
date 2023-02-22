-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2021 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Tomáš Souček (xsouce15)
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- ram[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_WREN  : out std_logic;                    -- cteni z pameti (DATA_WREN='0') / zapis do pameti (DATA_WREN='1')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WREN musi byt '0'
   OUT_WREN : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

	-- PC
	signal pc : std_logic_vector(11 downto 0);
	signal pcInc : std_logic;
	signal pcDec : std_logic;
	
	-- CNT
	signal cnt : std_logic_vector(7 downto 0);
	signal cntInc : std_logic;
	signal cntDec : std_logic;

	-- PTR
	signal ptr : std_logic_vector(9 downto 0);
	signal ptrInc : std_logic;
	signal ptrDec : std_logic;



	-- MUX
	-- 00...IN_DATA || 01...DATA_RDATA-1 || 10...DATA_RDATA+1
	signal mux : std_logic_vector(7 downto 0);
	signal muxSel : std_logic_vector(1 downto 0) := "00";

	-- FSM stavy
	type fsmState is (
		sInit,
		sFetch,
		sDecode,
		sPtrInc, -- >
		sPtrDec, -- <
		sValueInc, sValueInc1, sValueInc2, -- +
		sValueDec0, sValueDec1, sValueDec2, -- -
		sLoop0, sLoop1, sLoop2, sLoopEn, -- [
		sLoopStop0, sLoopStop1, sLoopStop2, sLoopStop3, sLoopStopEn, -- ]
		sWrite0, sWrite1, -- .
		sRead0, sRead1, --  ,
		sBreak0, sBreak1, sBreakEn, -- ~
		sNull -- null
	);
	signal pState : fsmState := sInit;
	signal nState : fsmState;

begin

	-- PC
	process (CLK, RESET, pcInc, pcDec)
	begin
		if RESET = '1' then
			pc <= "000000000000";
		elsif rising_edge(CLK) then
			if pcInc = '1' then
				pc <= pc + 1;
			elsif pcDec = '1' then
				pc <= pc - 1;
			end if;
		end if;
	end process;

	CODE_ADDR <= pc;


	-- PTR
	process (CLK, RESET, ptrInc, ptrDec)
	begin
		if RESET = '1' then
			ptr <= "0000000000";
		elsif rising_edge(CLK) then
			if ptrInc = '1' then
				ptr <= ptr + 1;
			elsif ptrDec = '1' then
				ptr <= ptr - 1;
			end if;
		end if;
	end process;

	DATA_ADDR <= ptr;


	-- CNT
	process (CLK, RESET, cntInc, cntDec)
	begin
		if RESET = '1' then
			cnt <= "00000000";
		elsif rising_edge(CLK) then
			if cntInc = '1' then
				cnt <= cnt + 1;
			elsif cntDec = '1' then
				cnt <= cnt - 1;
			end if;
		end if;
	end process;


	OUT_DATA <= DATA_RDATA;


	-- MUX
	process (CLK, RESET, muxSel)
	begin
		if RESET = '1' then
			mux <= "00000000";
		elsif rising_edge(CLK) then
			case muxSel is
				when "00" =>
					mux <= IN_DATA;

				when "01" =>
					mux <= DATA_RDATA - 1;

				when "10" =>
					mux <= DATA_RDATA + 1;

				when others =>
					mux <= DATA_RDATA;

			end case;
		end if;
	end process;

	DATA_WDATA <= mux;

	
	-- FSM
	-- aktualni stav
	process(RESET, CLK, EN)
	begin
		if (RESET='1') then
			pstate <= SInit;
		else
			if rising_edge(CLK) then
				if (EN='1') then 
					pstate <= nstate;
				end if;
			end if;
		end if;
	end process;


	-- nasledujici stav
	process(pstate, IN_VLD, OUT_BUSY, cnt, CODE_DATA, DATA_RDATA)
	begin
		OUT_WREN <= '0';
		DATA_WREN <= '0';
		IN_REQ <= '0';
		CODE_EN <= '0';
		DATA_EN <= '0';
		
		pcInc <= '0';
		pcDec <= '0';
		
		cntInc <= '0';
		cntDec <= '0';
		 
		ptrInc <= '0';
		ptrDec <= '0';
		
		muxSel <= "00";
	

		case pState is
			when sInit =>
				nState <= sFetch;
			
			--------------------------
			
			when sFetch =>
				CODE_EN <= '1';
				nState <= sDecode;
				
			--------------------------

			when sDecode =>
				case CODE_DATA is
					when X"3E" =>
						nState <= sPtrInc; -- >
						
					when X"3C" =>
						nState <= sPtrDec; -- <
						
					when X"2B" =>
						nState <= sValueInc; -- +
						
					when X"2D" =>
						nState <= sValueDec0; -- -
						
					when X"5B" =>
						nState <= sLoop0; -- [
						
					when X"5D" =>
						nState <= sLoopStop0; -- ]
						
					when X"2E" =>
						nState <= sWrite0; -- .
						
					when X"2C" =>
						nState <= sRead0; -- ,
						
					when X"7E" =>
						nState <= sBreak0; -- ~
						
					when X"00" =>
						nState <= sNull; -- null
						
					when others =>
						pcInc <= '1';
						nState <= sFetch;
				end case;

			--------------------------
			
			when sPtrInc =>
				ptrInc <= '1';
				pcInc <= '1';
				nState <= sFetch;
			
			--------------------------

			when sPtrDec =>
				ptrDec <= '1';
				pcInc <= '1';
				nState <= sFetch;
			
			--------------------------

			when sValueInc =>
				DATA_EN <= '1';
				nState <= sValueInc1;

			when sValueInc1 =>
				DATA_EN <= '1';
				muxSel <= "10"; --DATA_RDATA+1
				nState <= sValueInc2;

			when sValueInc2 =>
				DATA_EN <= '1';
				DATA_WREN <= '1';
				pcInc <= '1';
				nState <= sFetch;
			
			--------------------------

			when sValueDec0 =>
				DATA_EN <= '1';
				nState <= sValueDec1;

			when sValueDec1 =>
				DATA_EN <= '1';
				muxSel <= "01"; --DATA_RDATA-1
				nState <= sValueDec2;

			when sValueDec2 =>
				DATA_EN <= '1';
				DATA_WREN <= '1';
				pcInc <= '1';
				nState <= sFetch;
			
			--------------------------

			when sLoop0 =>
				DATA_EN <= '1'; -- povoleni cinnosti
				DATA_WREN <= '0'; -- nastaveni na cteni
				pcInc <= '1';
				nstate <= sLoop1;
				
			when sLoop1 =>
					if DATA_RDATA = (DATA_RDATA'range => '0') then
						cntInc <= '1';
						if cnt = (cnt'range => '0') then
							nState <= sFetch;
						else
							nState <= sLoopEn;
						end if;
					else
						nState <= sFetch;
					end if;
					
			when sLoop2 =>
				if cnt = (cnt'range => '0') then
						nState <= sFetch;
				else
					if CODE_DATA = X"5B" then -- [
							cntInc <= '1';
					elsif CODE_DATA = X"5D" then -- ]
							cntDec <= '1';
					end if;
					pcInc <= '1';
					nState <= sLoopEn;
				end if;
				
			when sLoopEn =>
				CODE_EN <= '1';
				nState <= sLoop2;

			--------------------------
			
			when sLoopStop0 =>
				DATA_EN <= '1';
				DATA_WREN <= '0';
				nState <= sLoopStop1;

			when sLoopStop1 =>
				if DATA_RDATA = (DATA_RDATA'range => '0') then
					pcInc <= '1';
					nState <= sFetch;
				else
					cntInc <= '1';
					pcDec <= '1';
					nState <= sLoopStopEn;
					
				end if;

			when sLoopStop2 =>
				if cnt = (cnt'range => '0') then
					nState <= sFetch;
				else
					if CODE_DATA = X"5D" then -- ]
						cntInc <= '1';
					elsif CODE_DATA = X"5B" then -- [
						cntDec <= '1';
					end if;

					nState <= sLoopStop3;
				end if;

			when sLoopStop3 =>
				if cnt = (cnt'range => '0') then
					pcInc <= '1';
				else
					pcDec <= '1';
				end if;

				nState <= sLoopStopEn;

			when sLoopStopEn =>
				CODE_EN <= '1';
				nState <= sLoopStop2;
				
			--------------------------

			when sWrite0 =>
				DATA_EN <= '1';
				DATA_WREN <= '0';
				nState <= sWrite1;

			when sWrite1 =>
				if OUT_BUSY = '1' then
					nState <= sWrite0;
				else
					OUT_WREN <= '1';
					pcInc <= '1';
					nState <= sFetch;
				end if;
				
			--------------------------
			
			when sRead0 =>
				IN_REQ <= '1';
				muxSel <= "00";
				nState <= sRead1;

			when sRead1 =>
				if IN_VLD = '1' then
					DATA_EN <= '1';
					DATA_WREN <= '1';
					pcInc <= '1';
					nState <= sFetch;
					
				else
					nState <= sRead0;
				end if;

			--------------------------
			
			when sBreak0 =>
				cntInc <= '1';
				pcInc <= '1';
				nState <= sBreakEn;

			when sBreak1 =>
				if cnt = (cnt'range => '0') then
					nState <= sFetch;
				else 
					if CODE_DATA = X"5B" then -- pokud je na vstupu [
						cntInc <= '1';
					elsif CODE_DATA = X"5D" then -- pokud je na vstupu ]
						cntDec <= '1';
					end if;

					pcInc <= '1';
					nState <= sBreakEn;
				end if;

			when sBreakEn =>
				CODE_EN <= '1';
				nState <= sBreak1;
			
			--------------------------

			when sNull =>
				nState <= sNull;

			when others =>	null;

		end case;
	end process;

end behavioral;