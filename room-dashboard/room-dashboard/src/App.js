import {CAlert, CBadge, CButton, CCard, CCardBody, CCardFooter, CCardHeader, CCol, CContainer, CFormRange, CFormSwitch,CRow} from '@coreui/react'
import '@coreui/coreui/dist/css/coreui.min.css'
import './App.css'
import { useEffect, useState } from 'react'
import { CChart } from '@coreui/react-chartjs'
import axios from 'axios'
import { format } from 'date-fns'
import  CIcon  from '@coreui/icons-react';
import { cilSync } from '@coreui/icons';

const App = () => {
  const [manageLight, setManageLight] = useState(true)
  const [manageRollerBlind, setManageRollerBlind] = useState(50)
  const [currentLightState, setCurrentLightState] = useState(null)
  const [currentRollerBlindState, setCurrentRollerBlindState] = useState(null)

  const [visibleAlert,setVisibleAlert] = useState(false);
  const [alertState,setAlertState] = useState(false);

  const [lightData, setLightData] = useState({
	label : [],
	data : []
  })

  const [rollerBlindData, setRollerBlindData] = useState({
	label : [],
	data : []
  })

  useEffect(() =>{
	if(lightData.data.length === 0 && lightData.label.length === 0 && rollerBlindData.data.length === 0 && rollerBlindData.label.length === 0){
		const interval = setInterval(() => requestChartData(), 1000)
		updateLightChart();
		updateRollerBlindChart();
		return () => clearInterval(interval)
	}
  }, [])

  const requestChartData = () => {
	axios.post('/current_state', {}, {
		headers: {
		  'Content-Type': 'multipart/form-data',
		  'Accept': 'application/json',
		},
	  }).then((response) => {
		console.log(response.data)
		if (response.data.success !== undefined) {
		  console.log(response.data.success.light);
		  console.log(response.data.success.roller_blind);
		  let light_data = response.data.success.light;
		  let roller_blind_data = response.data.success.roller_blind;
			setCurrentLightState(light_data);
			setCurrentRollerBlindState(roller_blind_data);
		}
	})
  }

  const changeStatus = () => {
	axios.post('/manage_room_state', { light: manageLight, rollerBlind : parseInt(manageRollerBlind) }, {
		headers: {
		  'Content-Type': 'application/json',
		  'Accept': 'application/json',
		},
	  }).then((response) => {
		if (response.data.success) {
			setAlertState(true);
			setVisibleAlert(true);
		} else {
			setAlertState(false);
			setVisibleAlert(true);
		}
	})
  }

  const updateLightChart = () => {
	axios.post('/update_light_chart', {}, {
		headers: {
		  'Content-Type': 'multipart/form-data',
		  'Accept': 'application/json',
		},
	  }).then((response) => {
		if (response.data.success !== undefined) {
		  console.log(response.data.success.light_data);
		  let light_data = response.data.success.light_data;
		  if(light_data.length > 0){
			let new_label = light_data.map(ld => format(new Date(ld.date),'yyyy-MM-dd HH:mm:ss'));
			let new_data = light_data.map(ld => ld.light ? 'ON' : 'OFF');
			let new_light_data = {
				label : new_label,
				data : new_data,
			}
			setLightData(new_light_data)
		  }
		}
	})
  }

  const updateRollerBlindChart = () => {
	axios.post('/update_roller_blind_chart', {}, {
		headers: {
		  'Content-Type': 'multipart/form-data',
		  'Accept': 'application/json',
		},
	  }).then((response) => {
		console.log(response.data)
		if (response.data.success !== undefined) {
		  console.log(response.data.success.roller_blind_data);
		  let roller_blind_data = response.data.success.roller_blind_data;
		  if(roller_blind_data.length > 0){
			let new_label = roller_blind_data.map(rbd => format(new Date(rbd.date),'yyyy-MM-dd HH:mm:ss'));
			let new_data = roller_blind_data.map(rbd => rbd.roller_blind);
			let new_roller_blind_data = {
				label : new_label,
				data : new_data,
			}
			setRollerBlindData(new_roller_blind_data);
		  }
		}
	})
  }

  return ( <>
		<CContainer fluid>
			<CRow>
				<h1 className='heading'>Smart Room Dashboard</h1>
			</CRow>
			<CRow>
				<CAlert color={alertState ? 'success' : 'danger'} dismissible visible={visibleAlert} onClose={() => setVisibleAlert(false)}>
					{alertState ? 'Room state changed successfully' : 'An error occurred while trying to change room state, retry'}
				</CAlert>
				<CCol>
					<CCard className='current-status'>
						<CCardHeader>
							<h2>Current Room Status</h2>
						</CCardHeader>
						<CCardBody>
							<p><b>Light Status:</b> <CBadge color={currentLightState === null ? 'warning' : currentLightState ? 'success' : 'danger'}>{currentLightState === null ? 'NULL' : currentLightState ? 'ON' : 'OFF'}</CBadge></p>
							<p><b>Roller Blind Status:</b> <CBadge color={currentRollerBlindState === null ? 'warning' : 'info'}>{currentRollerBlindState === null ? 'NULL' : currentRollerBlindState}</CBadge></p>
						</CCardBody>
					</CCard>
				</CCol>
				<CCol>
					<CCard className='manage-status'>
						<CCardHeader><h2>Manage Room Status</h2></CCardHeader>
						<CCardBody>
							<CFormRange
        						label={'Roller Blind Level\t' + manageRollerBlind}
        						min={0}
        						max={100}
        						step={1}
								value={manageRollerBlind}
								onChange={(event) => setManageRollerBlind(event.target.value)}
    						></CFormRange>
							<CFormSwitch
								size='lg'
								label='Light'
								defaultChecked
								onClick={(event) => setManageLight(event.target.checked)}
      						></CFormSwitch>
						</CCardBody>
						<CCardFooter>
							<CButton
								color='primary'
								shape='rounded'
								type='button'
								onClick={() => changeStatus()}
							>Save
							</CButton>
						</CCardFooter>
					</CCard>
				</CCol>
			</CRow>
			<CRow>
				<CCol>
					<CCard className='chart-light'>
						<CCardHeader>
							<CRow>
								<CCol xs={11}>
									<h2>Light Status Chart</h2>
								</CCol>
								<CCol xs={1}>
									<CButton onClick={() => updateLightChart()}><CIcon icon={cilSync} size="lg"/></CButton>
								</CCol>
							</CRow>
						</CCardHeader>
						<CCardBody>
							<CChart 
								type='line'
								options={{
									responsive : true,
									plugins: {
										title : {
											display : true,
											text : 'Light Status'
										}			
									},
									scales : {
										y : {
											type : 'category',
											labels : ['ON', 'OFF'],
											offset : true,
										}
									},
									animation : false,
									elements: {
										line: {
										  borderWidth: 2,
										  tension: 0,
										},
										point: {
										  radius: 4,
										  hitRadius: 10,
										  hoverRadius: 4,
										},
									},
								}}
								customTooltips={false}
								data={{
									labels : lightData.label,
									datasets : [
										{
											label: 'Status',
											data: lightData.data,
											backgroundColor: '#E46651',
                  							borderColor: '#E46651',
                  							pointHoverBackgroundColor: '#E46651',
      										stepped: true,
										}
									]
								}}
							></CChart>
						</CCardBody>
					</CCard>
				</CCol>
			</CRow>
			<CRow>
				<CCol>
					<CCard className='chart-roller'>
						<CCardHeader>
							<CRow>
								<CCol xs={11}>
									<h2>Roller Blind Status Chart</h2>
								</CCol>
								<CCol xs={1}>
									<CButton onClick={() => updateRollerBlindChart()}><CIcon icon={cilSync} size="lg"/></CButton>
								</CCol>
							</CRow>
						</CCardHeader>
						<CCardBody>
							<CChart 
								type='line'
								options={{
									responsive : true,
									plugins: {
										title : {
											display : true,
											text : 'Roller Blind Status'
										}			
									},
									scales : {
										y : {
											offset : true,
										}
									},
									animation : false,
									elements: {
										line: {
										  borderWidth: 2,
										  tension: 0,
										},
										point: {
										  radius: 4,
										  hitRadius: 10,
										  hoverRadius: 4,
										},
									},
								}}
								customTooltips={false}
								data={{
									labels : rollerBlindData.label,
									datasets : [
										{
											label: 'Status',
											data: rollerBlindData.data,
											backgroundColor: '#E46651',
                  							borderColor: '#E46651',
                  							pointHoverBackgroundColor: '#E46651',
										}
									]
								}}
							></CChart>
						</CCardBody>
					</CCard>
				</CCol>
			</CRow>
		</CContainer>
		</>
  );
}

export default App;
