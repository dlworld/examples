Name:		vdi-setup	
Version:	1.0.0
#Release:	0.el6
Release:	2%{?dist}
Summary:	vdi setup script

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
chmod +x ./bin/*
cp -aP ./*	%{buildroot}/
#install -m 0755 ./*	%{buildroot}/

%clean
rm -rf %{buildroot}
%files
%defattr(-,root,root,-)
%doc
/bin/*



%changelog
* Tue Dec 22 2015 sj<sunjun@os-easy.com> - 1.0.0 -0
- initial version
