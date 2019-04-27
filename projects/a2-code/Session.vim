let SessionLoad = 1
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
cd ~/Documents/school/19spring/4061/projects/a2-code
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
badd +16 bl_server.c
badd +470 term://.//25222:/usr/bin/zsh
badd +121 term://.//25223:/usr/bin/zsh
badd +55 bl_client.c
badd +142 server_funcs.c
badd +39 util.c
badd +56 blather.h
badd +19 term://.//25295:/usr/bin/zsh
badd +17 simpio_demo.c
badd +2 man://select(3posix)
badd +41 man://select(2)
badd +11 man://write(3posix)
badd +31 man://read(3posix)
badd +1 term://.//11899:/usr/bin/zsh
badd +27 term://.//4330:/usr/bin/zsh
badd +53 term://.//27716:/usr/bin/zsh
badd +145 man://signal(7)
badd +29 man://sigaction(2)
badd +29 man://sigaction(3posix)
badd +129 term://.//11898:/usr/bin/zsh
badd +38 man://timeval(3bsd)
badd +22 man://remove(3)
badd +9 man://strcpy(3)
badd +12 man://printf(3)
badd +118 test_blather.sh
badd +0 test-data/test-02-Bruce.out.diff
badd +2 test_blather_data.sh
badd +0 bl_client
argglobal
silent! argdel *
$argadd bl_server.c
set stal=2
set splitbelow splitright
wincmd _ | wincmd |
split
1wincmd k
wincmd w
wincmd t
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe '1resize ' . ((&lines * 27 + 29) / 59)
exe '2resize ' . ((&lines * 27 + 29) / 59)
argglobal
if bufexists("term://.//11898:/usr/bin/zsh") | buffer term://.//11898:/usr/bin/zsh | else | edit term://.//11898:/usr/bin/zsh | endif
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 3767 - ((12 * winheight(0) + 13) / 27)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
3767
normal! 045|
wincmd w
argglobal
if bufexists("term://.//11899:/usr/bin/zsh") | buffer term://.//11899:/usr/bin/zsh | else | edit term://.//11899:/usr/bin/zsh | endif
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 27 - ((26 * winheight(0) + 13) / 27)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
27
normal! 0
wincmd w
exe '1resize ' . ((&lines * 27 + 29) / 59)
exe '2resize ' . ((&lines * 27 + 29) / 59)
tabedit test_blather.sh
set splitbelow splitright
wincmd t
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 47 - ((46 * winheight(0) + 27) / 55)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
47
normal! 0
tabedit bl_client.c
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
wincmd t
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe 'vert 1resize ' . ((&columns * 106 + 106) / 213)
exe 'vert 2resize ' . ((&columns * 106 + 106) / 213)
argglobal
setlocal fdm=syntax
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=4
setlocal fml=1
setlocal fdn=20
setlocal fen
15
normal! zo
77
normal! zo
81
normal! zo
114
normal! zo
118
normal! zo
122
normal! zo
let s:l = 105 - ((23 * winheight(0) + 27) / 55)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
105
normal! 03|
wincmd w
argglobal
if bufexists("server_funcs.c") | buffer server_funcs.c | else | edit server_funcs.c | endif
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=5
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
7,10fold
6,12fold
39,41fold
24,47fold
60,62fold
66,68fold
56,75fold
88,91fold
84,109fold
127,129fold
117,131fold
140,143fold
139,145fold
166,168fold
178,182fold
185,188fold
184,189fold
184,189fold
184,189fold
153,190fold
196,198fold
204,207fold
214,216fold
203,228fold
233,235fold
249,252fold
260,263fold
264,267fold
268,271fold
248,274fold
6
normal! zo
7
normal! zc
6
normal! zc
24
normal! zo
39
normal! zc
24
normal! zc
56
normal! zo
60
normal! zc
66
normal! zc
56
normal! zc
84
normal! zo
88
normal! zc
84
normal! zc
117
normal! zo
127
normal! zc
117
normal! zc
139
normal! zo
140
normal! zc
139
normal! zc
153
normal! zo
166
normal! zc
178
normal! zc
184
normal! zo
184
normal! zo
184
normal! zo
185
normal! zc
184
normal! zc
184
normal! zc
153
normal! zc
196
normal! zc
203
normal! zo
204
normal! zc
214
normal! zc
203
normal! zc
233
normal! zc
248
normal! zo
249
normal! zc
260
normal! zc
264
normal! zc
268
normal! zc
let s:l = 279 - ((76 * winheight(0) + 27) / 55)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
279
normal! 0
wincmd w
exe 'vert 1resize ' . ((&columns * 106 + 106) / 213)
exe 'vert 2resize ' . ((&columns * 106 + 106) / 213)
tabnext 1
set stal=1
if exists('s:wipebuf') && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20 winminheight=1 winminwidth=1 shortmess=filnxtToOFcI
let s:sx = expand("<sfile>:p:r")."x.vim"
if file_readable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &so = s:so_save | let &siso = s:siso_save
let g:this_session = v:this_session
let g:this_obsession = v:this_session
let g:this_obsession_status = 2
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
