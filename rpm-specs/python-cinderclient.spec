Name:             python-cinderclient
Version:          1.4.0
Release:          11
Summary:          Python API and CLI for OpenStack Cinder

License:          ASL 2.0
URL:              http://github.com/openstack/python-cinderclient
Source0:          http://pypi.python.org/packages/source/p/%{name}/%{name}-%{version}.tar.gz

BuildArch:        noarch

BuildRequires:    python2-devel
BuildRequires:    python-setuptools
BuildRequires:    python-pbr
BuildRequires:    python-d2to1

Requires:         python-babel
Requires:         python-keystoneclient
Requires:         python-pbr
Requires:         python-prettytable
Requires:         python-requests
Requires:         python-setuptools
Requires:         python-simplejson
Requires:         python-six

%description
Client library (cinderclient python module) and command line utility
(cinder) for interacting with OpenStack Cinder (Block Storage) API.


%package doc
Summary:          Documentation for OpenStack Cinder API Client
Group:            Documentation

BuildRequires:    python-sphinx
BuildRequires:    python-oslo-sphinx

%description      doc
Client library (cinderclient python module) and command line utility
(cinder) for interacting with OpenStack Cinder (Block Storage) API.

This package contains auto-generated documentation.


%prep
%setup -q
%autosetup -n %{name}-%{version} -S git


# Let RPM handle the requirements
rm -f {,test-}requirements.txt

%build
%{__python2} setup.py build

%install
%{__python2} setup.py install -O1 --skip-build --root %{buildroot}

install -p -D -m 644 tools/cinder.bash_completion %{buildroot}%{_sysconfdir}/bash_completion.d/cinder.bash_completion

# Delete tests
rm -fr %{buildroot}%{python2_sitelib}/cinderclient/tests

export PYTHONPATH="$( pwd ):$PYTHONPATH"
sphinx-build -b html doc/source html
sphinx-build -b man doc/source man

install -p -D -m 644 man/cinder.1 %{buildroot}%{_mandir}/man1/cinder.1

compile_config="/root/cinderclient.conf"
python /root/deletepy.py %{buildroot} ${compile_config}

# Fix hidden-file-or-dir warnings
rm -fr html/.doctrees html/.buildinfo

%files
%doc LICENSE README.rst
%{_bindir}/cinder
%{python2_sitelib}/cinderclient
%{python2_sitelib}/*.egg-info
%{_sysconfdir}/bash_completion.d/cinder.bash_completion
%{_mandir}/man1/cinder.1*

%files doc
%doc html

%changelog
* Fri Sep 18 2015 Haikel Guemar <hguemar@fedoraproject.org> 1.4.0-1
- Update to upstream 1.4.0

* Tue Jul 21 2015 Haikel Guemar <hguemar@fedoraproject.org> 1.2.1-1
- Update to upstream 1.2.1

* Thu Jun 18 2015 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 1.1.1-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_23_Mass_Rebuild

* Mon Oct 13 2014 Jakub Ruzicka <jruzicka@redhat.com> 1.1.1-1
- Update to upstream 1.1.1
- New -doc BuildRequires: python-oslo-sphinx
- oslosphinx -> oslo.sphinx fix

* Sat Jun 07 2014 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 1.0.9-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_21_Mass_Rebuild

* Thu May 29 2014 Jakub Ruzicka <jruzicka@redhat.com> 1.0.9-1
- Update to upstream 1.0.9

* Tue Mar 25 2014 Jakub Ruzicka <jruzicka@redhat.com> 1.0.8-1
- Update to upstream 1.0.8
- Remove {test-,}requirements.txt in .spec instead of patch

* Thu Dec 19 2013 Jakub Ruzicka <jruzicka@redhat.com> 1.0.7-2
- Add search_opts into the method list() for VolumeTypeManager (rhbz#1048326)

* Wed Nov 06 2013 Jakub Ruzicka <jruzicka@redhat.com> 1.0.7-1
- Update to upstream version 1.0.7

* Wed Oct 23 2013 Jakub Ruzicka <jruzicka@redhat.com> 1.0.6-2
- Fix DeprecationWarning when printing exception
- Provide -doc package with auto-generated documentation
- Provide upstream manpage
- Improve package description

* Thu Oct 10 2013 Jakub Ruzicka <jruzicka@redhat.com> 1.0.6-1
- Update to upstream 1.0.6

* Thu Sep 19 2013 Jakub Ruzicka <jruzicka@redhat.com> 1.0.5-2
- Add update_snapshot_metadata action

* Thu Sep 12 2013 Jakub Ruzicka <jruzicka@redhat.com> 1.0.5-1
- Update to upstream version 1.0.5.
- Update dependencies.
- Remove runtime dependency on python-pbr.
- Change Source0 to PyPI.

* Sun Aug 04 2013 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 1.0.4-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_20_Mass_Rebuild

* Mon May 13 2013 Jakub Ruzicka <jruzicka@redhat.com> 1.0.4-1
- Update to upstream version 1.0.4.

* Tue Apr 02 2013 Jakub Ruzicka <jruzicka@redhat.com> 1.0.3-1
- Update to upstream version 1.0.3.

* Thu Feb 14 2013 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 1.0.2-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_19_Mass_Rebuild

* Mon Jan 14 2013 Eric Harney <eharney@redhat.com> 1.0.2-1
- Add bash completion support
- Update to latest client

* Tue Sep 25 2012 Pádraig Brady <P@draigBrady.com> 0.2.26-1
- Update to latest client to support latest cinder

* Mon Sep  3 2012 Pádraig Brady <P@draigBrady.com> 0.2-2
- Initial release
