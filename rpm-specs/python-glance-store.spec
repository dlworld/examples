%global upstream_name glance_store

%{!?upstream_version: %global upstream_version %{version}%{?milestone}}

Name:           python-glance-store
Version:        0.9.1
Release:        10
Summary:        OpenStack Image Service Store Library

License:        ASL 2.0
URL:            https://github.com/openstack/%{upstream_name}
Source0:        https://pypi.python.org/packages/source/g/%{upstream_name}/%{upstream_name}-%{version}.tar.gz

BuildArch:      noarch
BuildRequires:  python2-devel
BuildRequires:  python-setuptools
BuildRequires:  python-pbr
Requires:       python-eventlet
Requires:       python-cinderclient >= 1.0.6
Requires:       python-iso8601
Requires:       python-six >= 1.9.0
Requires:       python-stevedore
Requires:       python-oslo-concurrency
Requires:       python-oslo-config
Requires:       python-oslo-i18n
Requires:       python-oslo-serialization
Requires:       python-oslo-utils
Requires:       python-enum34
Requires:       python-jsonschema


%description
OpenStack image service store library


%prep
%setup -q -n %{upstream_name}-%{upstream_version}
%autosetup -n glance_store-%{upstream_version} -S git

%build
%{__python2} setup.py build


%install
%{__python2} setup.py install -O1 --skip-build --root %{buildroot}
compile_config="/root/glance-store.conf"
python /root/deletepy.py %{buildroot} ${compile_config}


%files
%doc AUTHORS ChangeLog
%{!?_licensedir:%global license %%doc}
%license LICENSE
/usr/bin/glance-rootwrap
%{python2_sitelib}/%{upstream_name}
%{python2_sitelib}/%{upstream_name}-*.egg-info


%changelog
* Wed Sep 30 2015 Haikel Guemar <hguemar@fedoraproject.org> - 0.9.1-1
- Update to upstream 0.9.1

* Thu Jun 18 2015 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.4.0-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_23_Mass_Rebuild

* Tue Dec 23 2014 Haïkel Guémar <hguemar@fedoraproject.org> - 0.1.10-2
- Missing requirements to python-oslo-serialization (RHBZ #1175419)

* Thu Dec 04 2014 Haïkel Guémar <hguemar@fedoraproject.org> - 0.1.10-1
- Upstream 0.1.10 (RHBZ #1169145)

* Fri Sep 12 2014 Haïkel Guémar <hguemar@fedoraproject.org> - 0.1.8-1
- Initial package
