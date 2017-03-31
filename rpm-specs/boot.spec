Name:		boot	
Version:	1.0.0
#Release:	0.el6
Release:	0%{?dist}
Summary:	boot script

Group:		System
License:	GPL
BuildArch:	x86_64
URL:		www.os-easy.com
Source0:	%{name}-%{version}.tar.gz
BuildRoot:	%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)
Requires:   audit logrotate rsyslog

%description
vserver setup 

%define __debug_install_post  \
  %{_rpmconfigdir}/find-debuginfo.sh %{?_find_debuginfo_opts} "%{_builddir}/%{?buildsubdir}" \
%{nil}


%prep
%setup -q


%build


%install
cd $RPM_BUILD_DIR/%{name}-%{version}
chmod +x ./bin/firstboot.sh
cp -aP ./*	%{buildroot}/
pushd %{buildroot}/etc/systemd/system/multi-user.target.wants
ln -s /usr/lib/systemd/system/boot.service boot.service
popd
#install -m 0755 ./*	%{buildroot}/

%clean
rm -rf %{buildroot}
%files
%defattr(-,root,root,-)
%doc
/bin/firstboot.sh
/usr/lib/systemd/system/boot.service
/etc/systemd/system/multi-user.target.wants/boot.service


%changelog
* Tue Dec 22 2015 sj<sunjun@os-easy.com> - 1.0.0 -0
- initial version
