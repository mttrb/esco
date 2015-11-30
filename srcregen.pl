#!/usr/bin/perl

my $header = q{/* esco - interpreter for a lot (in future) esoteric languages.
 * Copyright (C) 2007-2010, Zasenko Sergey
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
};
my @srcs = split /\s/, `find . -name "*.cpp"`;
push @srcs, split /\s/, `find . -name "*.h"`;
foreach $file (@srcs) {
    open F, $file;
    my @a = <F>;
    close F;
    chomp @a;
    my $i = 0;
    my $b = 0;
    open F, ">$file";
    for ($i=0; $i<@a; $i++) {
        unless ($b) {
            if ($a[$i]=~/\*\//) {
                $b = 1;
                print F $header;
            }
        }
        else {
            print F "$a[$i]\n";
        }
    }
    print F "\n";
    close F;
}

