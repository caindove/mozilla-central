#!/usr/bin/perl -wT
# -*- Mode: perl; indent-tabs-mode: nil -*-
#
# The contents of this file are subject to the Mozilla Public
# License Version 1.1 (the "License"); you may not use this file
# except in compliance with the License. You may obtain a copy of
# the License at http://www.mozilla.org/MPL/
#
# Software distributed under the License is distributed on an "AS
# IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
# implied. See the License for the specific language governing
# rights and limitations under the License.
#
# The Original Code is the Bugzilla Test Runner System.
#
# The Initial Developer of the Original Code is Maciej Maczynski.
# Portions created by Maciej Maczynski are Copyright (C) 2001
# Maciej Maczynski. All Rights Reserved.
#
# Contributor(s): Greg Hendricks <ghendricks@novell.com>

use strict;
use lib ".";

use Bugzilla;
use Bugzilla::Util;
use Bugzilla::Constants;
use Bugzilla::Error;
use Bugzilla::Config;
use Bugzilla::Testopia::TestPlan;
use Bugzilla::Testopia::Build;
use Bugzilla::Testopia::Search;
use Bugzilla::Testopia::Table;
use Bugzilla::Testopia::Util;

Bugzilla->login(LOGIN_REQUIRED);
use vars qw($vars);

my $template = Bugzilla->template;
my $cgi = Bugzilla->cgi;
print $cgi->header;
$vars->{'txt'} = $cgi->param('text');
    $template->process("testopia/text.png.tmpl", $vars) 
      || ThrowTemplateError($template->error());