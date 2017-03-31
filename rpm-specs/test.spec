Name:		test
Version:	1.0.0
Release:	5%{?dist}
Summary:	test

Group:		System
License:	GPL
BuildArch:	x86_64
URL:		www.os-easy.com
Source0:	%{name}-%{version}.tar.gz
Source1:	test.service
#Requires:  

%description


%prep
%setup -q


%build


%install
install -D -p -m 0644 %{SOURCE1} $RPM_BUILD_ROOT%{_unitdir}/test.service
install -D -p -m 0644 test.conf	%{buildroot}/etc/test.conf
install -D -p -m 0755 test.sh	%{buildroot}/bin/test.sh

%pre

%post
systemctl daemon-reload
%systemd_post test.service
if systemctl is-enabled test.service > /dev/null; then
    systemctl restart test.service
fi


%preun
%systemd_preun test.service

%postun
%systemd_postun_with_restart test.service


%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
%doc
%config(noreplace) /etc/test.conf
%{_unitdir}/test.service
/bin/test.sh


%changelog
* Tue Mar 7 2017 dlw<denglinwen@os-easy.com> - 1.0.0 -0
- initial version
